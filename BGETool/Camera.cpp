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

#include "Camera.h"

typedef char *(__fastcall *OriginalCameraCopy)(char *pThis);

typedef DWORD *(__cdecl *DemoCameraCopy)(DWORD *a1, DWORD *a2, int a3, int a4, int a5);

OriginalCameraCopy pOriginalCameraCopy;
DemoCameraCopy pDemoCameraCopy;

bool *bFreecam_;

vec3f *Position_;
Offsets *offsets_;

char *__fastcall HookedCameraCopy(char *pThis)
{
    char *eax_;
    __asm mov eax_, eax

    if (*bFreecam_)
    {
        switch (offsets_->bgeVersion)
        {
            case Version_GOG_2109:
            case Version_Polish:
            {
                pOriginalCameraCopy(pThis);

                auto *x = (float *) (pThis + 0x30);
                auto *z = (float *) (pThis + 0x34);
                auto *y = (float *) (pThis + 0x38);
                *x = Position_->X;
                *z = Position_->Z;
                *y = Position_->Y;

                return pThis;
            }
            case Version_Uplay:
            case Version_Steam_Steamless:
            {
                __asm mov eax, eax_
                __asm mov ecx, pThis
                pOriginalCameraCopy(pThis);

                char *camPosStruct = (char *) mem::FindDMAAddy((uintptr_t) offsets_->GetOffset(OFFSET_CAMERA_POSITION),
                                                               {0x70, 0x00});

                if (camPosStruct == pThis)
                {
                    auto *x = (float *) (pThis + 0x30);
                    auto *z = (float *) (pThis + 0x34);
                    auto *y = (float *) (pThis + 0x38);
                    *x = Position_->X;
                    *z = Position_->Z;
                    *y = Position_->Y;
                }

                return eax_;
            }
        }
    }
    else
    {
        auto offset_camera_position = (uintptr_t) offsets_->GetOffset(OFFSET_CAMERA_POSITION);
        Position_->X = *((float *) mem::FindDMAAddy(offset_camera_position, {0x70, 0x30}));
        Position_->Z = *((float *) mem::FindDMAAddy(offset_camera_position, {0x70, 0x34}));
        Position_->Y = *((float *) mem::FindDMAAddy(offset_camera_position, {0x70, 0x38}));
    }

    if (offsets_->bgeVersion == Version_GOG_2109)
        return pOriginalCameraCopy(pThis);
    else
    {
        __asm mov eax, eax_
        __asm mov ecx, pThis
        pOriginalCameraCopy(pThis);

        return eax_;
    }
}

DWORD *__cdecl HookedDemoCameraCopy(DWORD *a1, DWORD *a2, int a3, int a4, int a5)
{
    if (*bFreecam_)
    {
        pDemoCameraCopy(a1, a2, a3, a4, a5);

        char *camStruct = (char *) a1;

        auto *x = (float *) (camStruct + 0x30);
        auto *z = (float *) (camStruct + 0x34);
        auto *y = (float *) (camStruct + 0x38);
        *x = Position_->X;
        *z = Position_->Z;
        *y = Position_->Y;

        return a1;
    }
    else
    {
        auto offset_camera_position = (uintptr_t) offsets_->GetOffset(OFFSET_CAMERA_POSITION);
        Position_->X = *((float *) mem::FindDMAAddy(offset_camera_position, {0x70, 0x30}));
        Position_->Z = *((float *) mem::FindDMAAddy(offset_camera_position, {0x70, 0x34}));
        Position_->Y = *((float *) mem::FindDMAAddy(offset_camera_position, {0x70, 0x38}));
    }

    return pDemoCameraCopy(a1, a2, a3, a4, a5);
}

bool *modifiedKeyboardState = new bool[256];

Camera::Camera(Offsets *offsets, Input *input)
{
    this->offsets = offsets;
    offsets_ = offsets;

    memset(modifiedKeyboardState, 0, 256);

    if (this->offsets->bgeVersion != Version_Demo)
    {
        pOriginalCameraCopy = (OriginalCameraCopy) (this->offsets->GetOffset(OFFSET_CAMERA_STRUCT_COPY_FUNCTION));
    }
    else
    {
        pDemoCameraCopy = (DemoCameraCopy) (this->offsets->GetOffset(OFFSET_CAMERA_STRUCT_COPY_FUNCTION));
    }

    this->bFreecam = false;
    bFreecam_ = &this->bFreecam;

    this->fSpeed = 0.1f;
    this->input = input;
    Position_ = &this->Position;
}

void Camera::HookCopy()
{
    if (this->offsets->bgeVersion != Version_Demo)
    {
        DetourTransactionBegin();
        DetourAttach(&(PVOID &) pOriginalCameraCopy, HookedCameraCopy);
        DetourTransactionCommit();
    }
    else
    {
        DetourTransactionBegin();
        DetourAttach(&(PVOID &) pDemoCameraCopy, HookedDemoCameraCopy);
        DetourTransactionCommit();
    }
}

void Camera::UnhookCopy()
{
    if (this->offsets->bgeVersion != Version_Demo)
    {
        DetourTransactionBegin();
        DetourDetach(&(PVOID &) pOriginalCameraCopy, HookedCameraCopy);
        DetourTransactionCommit();
    }
    else
    {
        DetourTransactionBegin();
        DetourDetach(&(PVOID &) pDemoCameraCopy, HookedDemoCameraCopy);
        DetourTransactionCommit();
    }
}

void Camera::Update()
{
    if (this->bFreecam)
    {
        memcpy(modifiedKeyboardState, &this->input->KeyboardState, 256);
        modifiedKeyboardState[DIK_W] = false;
        modifiedKeyboardState[DIK_A] = false;
        modifiedKeyboardState[DIK_S] = false;
        modifiedKeyboardState[DIK_D] = false;
        modifiedKeyboardState[DIK_Q] = false;
        modifiedKeyboardState[DIK_E] = false;
        modifiedKeyboardState[DIK_SPACE] = false;

        keyboardState = modifiedKeyboardState;
    }
    else
    {
        keyboardState = this->input->KeyboardState;
    }

    if (GetAsyncKeyState(VK_DELETE) & 1)
        this->ToggleFreecam();

    if (GetAsyncKeyState(VK_OEM_MINUS) & 1)
    {
        this->fSpeed -= 0.1f;
        cout << "Speed: " << this->fSpeed << "\n";
    }

    if (GetAsyncKeyState(VK_OEM_PLUS) & 1)
    {
        this->fSpeed += 0.1f;
        cout << "Speed: " << this->fSpeed << "\n";
    }

    if (this->bFreecam)
    {
        if (this->input->KeyboardState[DIK_Q])
            Position.Y -= this->fSpeed;
        if (this->input->KeyboardState[DIK_E])
            Position.Y += this->fSpeed;
        if (this->input->KeyboardState[DIK_A])
            Position.X -= this->fSpeed;
        if (this->input->KeyboardState[DIK_D])
            Position.X += this->fSpeed;
        if (this->input->KeyboardState[DIK_W])
            Position.Z += this->fSpeed;
        if (this->input->KeyboardState[DIK_S])
            Position.Z -= this->fSpeed;

        if (this->input->KeyboardState[DIK_SPACE])
        {
            auto offset_player_pos = (uintptr_t) this->offsets->GetOffset(OFFSET_PLAYER_POSITION);

            float *playerX_1 = (float *) mem::FindDMAAddy(offset_player_pos, {0x30});
            float *playerX_2 = (float *) mem::FindDMAAddy(offset_player_pos, {0xD0});

            float *playerZ_1 = (float *) mem::FindDMAAddy(offset_player_pos, {0x34});
            float *playerZ_2 = (float *) mem::FindDMAAddy(offset_player_pos, {0xD4});

            float *playerY_1 = (float *) mem::FindDMAAddy(offset_player_pos, {0x38});
            float *playerY_2 = (float *) mem::FindDMAAddy(offset_player_pos, {0xD8});

            *playerX_1 = Position.X;
            *playerX_2 = Position.X;
            *playerZ_1 = Position.Z;
            *playerZ_2 = Position.Z;
            *playerY_1 = Position.Y;
            *playerY_2 = Position.Y;

            cout << "Teleported to: " << Position.X << ", " << Position.Y << ", " << Position.Z << "\n";
        }
    }
}

void Camera::ToggleFreecam()
{
    bFreecam = !bFreecam;

    cout << (bFreecam ? "Enabled Freecam!" : "Disabled Freecam!") << endl;
}