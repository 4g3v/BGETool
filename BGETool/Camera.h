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

#ifndef BGETOOL_CAMERA_H
#define BGETOOL_CAMERA_H

#include <iostream>
#include <dinput.h>
#include <detours.h>
#include "Structures.h"
#include "mem.h"
#include "Input.h"

using namespace std;

class Camera
{
private:
    Offsets *offsets;
    Input *input;
    bool bFreecam;
    float fSpeed;

    void ToggleFreecam();

public:
    vec3f Position;

    Camera(Offsets *offsets, Input *input);

    void HookCopy();

    void UnhookCopy();

    void Update();
};

#endif //BGETOOL_CAMERA_H