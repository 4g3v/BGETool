// BGETool - BGETool mostly consists of a DLL which is being injected into Beyond Good and Evil. It allows for things like a Freecam, Teleportation and a re-enabled developer debug console aswell as potentially much more.
// Copyright (C) 2019  4g3v
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <algorithm>
#include <detours.h>
#include <d3d9.h>
#include <string>
#include "Structures.h"
#include "Offsets.h"
#include "mem.h"
#include "main.h"

using namespace std;

#define VERSION "0.10"

Offsets *offsets__;

Settings settings;

string GetIniPath()
{
    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    string iniPath = string(buffer);
    iniPath = iniPath.substr(0, iniPath.find_last_of('\\')).append("\\BGETool.ini");

    return iniPath;
}

string GetSetting(const char *key, const char *def)
{
    char buf[64];
    GetPrivateProfileStringA("Settings", key, def, (LPSTR) &buf, 64, GetIniPath().c_str());

    return string(buf);
}

UINT GetSetting(const char *key, int def)
{
    return GetPrivateProfileIntA("Settings", key, def, GetIniPath().c_str());
}

//Thanks to madshi on http://forum.madshi.net/viewtopic.php?p=44424&sid=30c61f2684495a91b21796c97d7b8ffe#p44424
PVOID GetInterfaceMethod_(PVOID intf, DWORD methodIndex)
{
    return *(PVOID *) (*(ULONG_PTR *) intf + methodIndex * sizeof(PVOID));
}

static HRESULT
(__stdcall *OriginalCreateDevice)(IDirect3D9 *iDirect3D9, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,
                                  DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters,
                                  IDirect3DDevice9 **ppReturnedDeviceInterface);

static UINT *(*OriginalD3DCreate)();

HRESULT __stdcall HookedCreateDevice(IDirect3D9 *iDirect3D9, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow,
                                     DWORD BehaviorFlags, D3DPRESENT_PARAMETERS *pPresentationParameters,
                                     IDirect3DDevice9 **ppReturnedDeviceInterface)
{
    pPresentationParameters->BackBufferWidth = settings.ResolutionX;
    pPresentationParameters->BackBufferHeight = settings.ResolutionY;
    if (settings.Windowed)
    {
        pPresentationParameters->Windowed = TRUE;
        pPresentationParameters->FullScreen_RefreshRateInHz = 0;
    }

    int *resX = (int *) offsets__->GetOffset(OFFSET_VIEWPORT_RESOLUTION_X);
    int *resY = (int *) offsets__->GetOffset(OFFSET_VIEWPORT_RESOLUTION_Y);
    *resX = settings.ResolutionX;
    *resY = settings.ResolutionY;
    cout << "Patched viewport resolution to " << dec << settings.ResolutionX << "x"
         << dec << settings.ResolutionY << endl;

    string windowMode = settings.Windowed ? "windowed" : "fullscreen";
    cout << "Patched CreateDevice to run in " << windowMode << " mode at " << dec << settings.ResolutionX << "x"
         << dec << settings.ResolutionY << endl;

    HRESULT ret = OriginalCreateDevice(iDirect3D9, Adapter, DeviceType, hFocusWindow, BehaviorFlags,
                                       pPresentationParameters, ppReturnedDeviceInterface);

    return ret;
}

UINT *D3DCreate()
{
    UINT *ret = OriginalD3DCreate();
    IDirect3D9 *iDirect3D9 = *(IDirect3D9 **) offsets__->GetOffset(OFFSET_IDIRECT3D9);

    PVOID createDevice = GetInterfaceMethod_(iDirect3D9, 16);
    cout << "CreateDevice is at: 0x" << createDevice << endl;
    OriginalCreateDevice = static_cast<HRESULT (__stdcall *)(IDirect3D9 *, UINT, D3DDEVTYPE, HWND, DWORD,
                                                             D3DPRESENT_PARAMETERS *,
                                                             IDirect3DDevice9 **)>(createDevice);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) OriginalD3DCreate, D3DCreate);
    DetourAttach(&(PVOID &) OriginalCreateDevice, HookedCreateDevice);
    DetourTransactionCommit();

    return ret;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            AllocConsole();
            FILE *stdoutFD;
            freopen_s(&stdoutFD, "CONOUT$", "w", stdout);

            cout << "BGETool v" << VERSION << " | Made by 4g3v | 4g3v@protonmail.com | Discord: Vega#0137" << endl
                 << endl;

            string windowed = GetSetting("Windowed", "false");
            transform(windowed.begin(), windowed.end(), windowed.begin(), ::tolower);

            settings.Windowed = windowed == "true";
            settings.ResolutionX = GetSetting("ResolutionX", 1280);
            settings.ResolutionY = GetSetting("ResolutionY", 720);

            auto *offsets = new Offsets();
            offsets__ = offsets;

            mem::PatchByte((BYTE *) offsets->GetOffset(OFFSET_DINPUT_COOPERATIVELEVEL), 0x06);
            cout << "Patched DInput keyboard CooperativeLevel!" << endl;

            mem::PatchInt((int *) offsets->GetOffset(OFFSET_WINDOW_X), settings.ResolutionX);
            mem::PatchInt((int *) offsets->GetOffset(OFFSET_WINDOW_Y), settings.ResolutionY);
            cout << "Patched window resolution!" << endl;

            cout << "Looking for BGE's main thread" << endl;

            DWORD currentThreadID = GetCurrentThreadId();
            DWORD currentProcessID = GetCurrentProcessId();
            DWORD bgeMainThreadID = 0;
            HANDLE th32 = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
            if (th32 != INVALID_HANDLE_VALUE)
            {
                THREADENTRY32 threadEntry;
                threadEntry.dwSize = sizeof(threadEntry);

                if (Thread32First(th32, &threadEntry))
                {
                    do
                    {
                        if (threadEntry.dwSize >=
                            FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(threadEntry.th32OwnerProcessID))
                        {
                            if (threadEntry.th32OwnerProcessID != currentProcessID)
                                continue;

                            if (threadEntry.th32ThreadID != currentThreadID)
                            {
                                bgeMainThreadID = threadEntry.th32ThreadID;
                                cout << "Found main thread: " << bgeMainThreadID << endl;
                                break;
                            }
                        }
                        threadEntry.dwSize = sizeof(threadEntry);
                    } while (Thread32Next(th32, &threadEntry));
                }

                CloseHandle(th32);
            }
            else
            {
                cout << "Couldn't create Toolhelp32Snapshot!" << endl;
                FatalError();
            }

            if (bgeMainThreadID == 0)
            {
                cout << "Couldn't find BGE's main thread!" << endl;
                FatalError();
            }

            OriginalD3DCreate = static_cast<UINT *(*)(void)>(offsets->GetOffset(OFFSET_D3DCREATE));
            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            DetourAttach(&(PVOID &) OriginalD3DCreate, D3DCreate);
            DetourTransactionCommit();

            cout << "Resuming main thread!" << endl << endl;
            HANDLE bgeMainThread = OpenThread(THREAD_ALL_ACCESS, FALSE, bgeMainThreadID);
            if (!bgeMainThread)
            {
                cout << "Couldn't open main thread!" << endl;
                FatalError();
            }

            ResumeThread(bgeMainThread);
            CloseHandle(bgeMainThread);

            auto *threadParameter = (ThreadParameter *) malloc(sizeof(ThreadParameter));
            threadParameter->hModule = hModule;
            threadParameter->stdoutFD = stdoutFD;
            threadParameter->offsets = offsets;

            CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE) BGETool, threadParameter, 0, nullptr));
            break;
        }
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}

