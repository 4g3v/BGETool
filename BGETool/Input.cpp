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

#include "Input.h"

bool *keyboardState;

Input::Input(Offsets *offsets)
{
    this->offsets = offsets;
    this->dInputDevice_Mouse = (*(IDirectInputDeviceA **) this->offsets->GetOffset(OFFSET_DINPUTDEVICE_MOUSE));
    this->dInputDevice_Keyboard = (*(IDirectInputDeviceA **) this->offsets->GetOffset(OFFSET_DINPUTDEVICE_KEYBOARD));
    keyboardState = this->KeyboardState;

    memset(&this->MouseState, 0, sizeof(DIMOUSESTATE2));
    memset(&this->KeyboardState, 0, 256);
}

//Thanks to madshi on http://forum.madshi.net/viewtopic.php?f=7&t=27248
PVOID GetInterfaceMethod(PVOID intf, DWORD methodIndex)
{
    return *(PVOID *) (*(ULONG_PTR *) intf + methodIndex * sizeof(PVOID));
}

static HRESULT (__stdcall *OriginalGetDeviceState)(IDirectInputDeviceA *device, DWORD cbData, LPVOID lpvData);

bool blockX;
bool blockY;

HRESULT __stdcall HookedGetDeviceState(IDirectInputDeviceA *device, DWORD cbData, LPVOID lpvData)
{
    HRESULT hr = OriginalGetDeviceState(device, cbData, lpvData);

    if (cbData == 20)
    {
        auto *mouse = (DIMOUSESTATE2 *) lpvData;
        if (blockY)
            mouse->lY = 0;
        if (blockX)
            mouse->lX = 0;
    }
    else if (cbData == 256)
    {
        memcpy(lpvData, keyboardState, 256);
    }
    else
        std::cout << "Unknown DirectInputDevice or focus was lost!" << std::endl;

    return hr;
}

void Input::HookDInput()
{
    PVOID getDeviceState = GetInterfaceMethod(this->dInputDevice_Mouse, 9);
    std::cout << "GetDeviceState is at: 0x" << getDeviceState << std::endl;
    OriginalGetDeviceState = static_cast<HRESULT (__stdcall *)(IDirectInputDeviceA *, DWORD, LPVOID)>(getDeviceState);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) OriginalGetDeviceState, HookedGetDeviceState);
    DetourTransactionCommit();
}

void Input::UnhookDInput()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) OriginalGetDeviceState, HookedGetDeviceState);
    DetourTransactionCommit();
}

void Input::Update()
{
    Sleep(25);
    OriginalGetDeviceState(this->dInputDevice_Mouse, sizeof(this->MouseState), &this->MouseState);
    OriginalGetDeviceState(this->dInputDevice_Keyboard, sizeof(this->KeyboardState), &this->KeyboardState);

    //if (GetAsyncKeyState(VK_F2) & 1)
    //{
    //    blockY = !blockY;
    //}
    //if (GetAsyncKeyState(VK_F3) & 1)
    //{
    //    blockX = !blockX;
    //}
}