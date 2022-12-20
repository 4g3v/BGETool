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

#ifndef BGETOOLSOLUTION_OFFSETS_H
#define BGETOOLSOLUTION_OFFSETS_H

#include <iostream>
#include <Windows.h>
#include <map>
#include "magic_enum.hpp"
#include "error.h"

using namespace std;

enum BGEVersion
{
    Version_GOG_2109,
    Version_Steam_Steamless,
    Version_Demo,
    Version_Uplay,
    Version_Polish
};

enum BGEOffset
{
    OFFSET_D3DCREATE,
    OFFSET_IDIRECT3D9,
    OFFSET_DINPUT_COOPERATIVELEVEL,
    OFFSET_DINPUTDEVICE_MOUSE,
    OFFSET_DINPUTDEVICE_KEYBOARD,
    OFFSET_WINDOW_HANDLE,
    OFFSET_WINDOW_X,
    OFFSET_WINDOW_Y,
    OFFSET_VIEWPORT_RESOLUTION_X,
    OFFSET_VIEWPORT_RESOLUTION_Y,
    OFFSET_DEBUG_CONSOLE_DISABLED,
    OFFSET_CAMERA_POSITION,
    OFFSET_PLAYER_POSITION,
    OFFSET_CAMERA_STRUCT_COPY_FUNCTION
};

class Offsets
{
private:
    map<BGEOffset, void *> offsets;
public:
    BGEVersion bgeVersion;

    Offsets();

    void *GetOffset(BGEOffset bgeOffset);
};

#endif //BGETOOLSOLUTION_OFFSETS_H
