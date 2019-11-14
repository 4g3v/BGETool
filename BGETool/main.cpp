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

#include <iostream>
#include <Windows.h>
#include <detours.h>
#include "mem.h"
#include "Input.h"
#include "Camera.h"

using namespace std;

DWORD WINAPI BGETool(ThreadParameter *threadParameter)
{
    while (!(GetAsyncKeyState(VK_DELETE) & 1))
        Sleep(10);

    MessageBeep(MB_ICONASTERISK);

    mem::PatchInt((int *) threadParameter->offsets->GetOffset(OFFSET_DEBUG_CONSOLE_DISABLED), 0x00);

    HWND *windowHandle = (HWND *) threadParameter->offsets->GetOffset(OFFSET_WINDOW_HANDLE);

    auto input = Input(threadParameter->offsets);
    input.HookDInput();

    auto cam = Camera(threadParameter->offsets, &input);
    cam.HookCopy();

    while (true)
    {
        input.Update();
        cam.Update();

        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            cout << "Minimizing window." << endl;
            ShowWindow(*windowHandle, SW_MINIMIZE);
        }

//        if (GetAsyncKeyState(VK_F10) & 1)
//        {
//            vec3f pos;
//
//            auto offset_camera_position = (uintptr_t) threadParameter->offsets->GetOffset(OFFSET_CAMERA_POSITION);
//            pos.X = *((float *) mem::FindDMAAddy(offset_camera_position, {0x70, 0x30}));
//            pos.Z = *((float *) mem::FindDMAAddy(offset_camera_position, {0x70, 0x34}));
//            pos.Y = *((float *) mem::FindDMAAddy(offset_camera_position, {0x70, 0x38}));
//            cout << "Camera is at: " << pos.X << ", " << pos.Y << ", " << pos.Z << "\n";
//
//            auto offset_player_position = (uintptr_t) threadParameter->offsets->GetOffset(OFFSET_PLAYER_POSITION);
//            pos.X = *((float *) mem::FindDMAAddy(offset_player_position, {0x30}));
//            pos.Z = *((float *) mem::FindDMAAddy(offset_player_position, {0x34}));
//            pos.Y = *((float *) mem::FindDMAAddy(offset_player_position, {0x38}));
//            cout << "Player1 is at: " << pos.X << ", " << pos.Y << ", " << pos.Z << "\n";
//
//            pos.X = *((float *) mem::FindDMAAddy(offset_player_position, {0xD0}));
//            pos.Z = *((float *) mem::FindDMAAddy(offset_player_position, {0xD4}));
//            pos.Y = *((float *) mem::FindDMAAddy(offset_player_position, {0xD8}));
//            cout << "Player2 is at: " << pos.X << ", " << pos.Y << ", " << pos.Z << "\n";
//        }
    }

    input.UnhookDInput();
    cam.UnhookCopy();

    fclose(threadParameter->stdoutFD);
    FreeConsole();
    FreeLibraryAndExitThread(threadParameter->hModule, 0);

    return 0;
}