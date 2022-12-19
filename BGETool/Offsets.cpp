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

#include "Offsets.h"

//Thanks to Mark Adler (https://stackoverflow.com/questions/27939882/fast-crc-algorithm)
#define POLY 0xedb88320

uint32_t crc32c(uint32_t crc, const unsigned char *buf, size_t len)
{
    int k;

    crc = ~crc;
    while (len--)
    {
        crc ^= *buf++;
        for (k = 0; k < 8; k++)
            crc = crc & 1 ? (crc >> 1) ^ POLY : crc >> 1;
    }
    return ~crc;
}

Offsets::Offsets()
{
    char bgePathBuf[MAX_PATH];
    GetModuleFileName(nullptr, bgePathBuf, MAX_PATH);

    HANDLE file = CreateFileA(bgePathBuf, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
                              nullptr);
    DWORD bgeSize = GetFileSize(file, nullptr);

    void *buf = malloc(bgeSize);
    DWORD readBytes = 0;
    ReadFile(file, buf, bgeSize, &readBytes, nullptr);
    CloseHandle(file);

    auto crc = crc32c(0, (const unsigned char *) buf, bgeSize);
    cout << "BGE CRC32: " << hex << crc << endl;

    switch (crc)
    {
        case 0x93c833bf:
            this->bgeVersion = Version_GOG_2109;
            break;
        case 0xa75c397:
            this->bgeVersion = Version_Steam_Steamless;
            break;
        case 0x5b8f8085:
            cout << "Detected packed Steam version. Please use Steamless to unpack this version!" << endl;
            cout << "Instructions can be found on the Github page." << endl;
            FatalError();
            break;
        case 0x654b60d4:
            this->bgeVersion = Version_Demo;
            break;
        case 0xded67699:
            this->bgeVersion = Version_Uplay;
            break;
        case 0xfb6ddd20:
            this->bgeVersion = Version_Polish;
            break;
        default:
            cout << "Unsupported BGE version! Please share your BGE.exe with me and have some patience :)" << endl;
            FatalError();
            break;
    }

    cout << "Found BGE version: " << magic_enum::enum_name(this->bgeVersion) << endl << endl;

    switch (this->bgeVersion)
    {
        case Version_GOG_2109:
            this->offsets[OFFSET_D3DCREATE] = (void *) 0x004027A0;
            this->offsets[OFFSET_IDIRECT3D9] = (void *) 0x00B8B600;
            this->offsets[OFFSET_DINPUT_COOPERATIVELEVEL] = (void *) 0x00422BCF;
            this->offsets[OFFSET_DINPUTDEVICE_MOUSE] = (void *) 0x00B7A798;
            this->offsets[OFFSET_DINPUTDEVICE_KEYBOARD] = (void *) 0x00B7A788;
            this->offsets[OFFSET_WINDOW_HANDLE] = (void *) 0x00D13220;
            this->offsets[OFFSET_WINDOW_X] = (void *) 0x00401CB1;
            this->offsets[OFFSET_WINDOW_Y] = (void *) 0x00401CB9;
            this->offsets[OFFSET_VIEWPORT_RESOLUTION_X] = (void *) 0x00B8B788;
            this->offsets[OFFSET_VIEWPORT_RESOLUTION_Y] = (void *) 0x00B8B78C;
            this->offsets[OFFSET_DEBUG_CONSOLE_DISABLED] = (void *) 0x00B8B5F8;
            this->offsets[OFFSET_CAMERA_POSITION] = (void *) 0xb7be74;
            this->offsets[OFFSET_PLAYER_POSITION] = (void *) 0xb79e14;
            this->offsets[OFFSET_CAMERA_STRUCT_COPY_FUNCTION] = (void *) 0x45E220;
            break;
        case Version_Uplay:
        case Version_Steam_Steamless:
            this->offsets[OFFSET_D3DCREATE] = (void *) 0x00AF2020;
            this->offsets[OFFSET_IDIRECT3D9] = (void *) 0x00BAAA80;
            this->offsets[OFFSET_DINPUT_COOPERATIVELEVEL] = (void *) 0x00B079AF;
            this->offsets[OFFSET_DINPUTDEVICE_MOUSE] = (void *) 0x00BA1AF4;
            this->offsets[OFFSET_DINPUTDEVICE_KEYBOARD] = (void *) 0x00BA1AE4;
            this->offsets[OFFSET_WINDOW_HANDLE] = (void *) 0x00BE2FE0;
            this->offsets[OFFSET_WINDOW_X] = (void *) 0x00401CA1;
            this->offsets[OFFSET_WINDOW_Y] = (void *) 0x00401CA9;
            this->offsets[OFFSET_VIEWPORT_RESOLUTION_X] = (void *) 0x00BAAC08;
            this->offsets[OFFSET_VIEWPORT_RESOLUTION_Y] = (void *) 0x00BAAC0C;
            this->offsets[OFFSET_DEBUG_CONSOLE_DISABLED] = (void *) 0x00BAAA78;
            this->offsets[OFFSET_CAMERA_POSITION] = (void *) 0x00B92564;
            this->offsets[OFFSET_PLAYER_POSITION] = (void *) 0x00B91BD4;
            this->offsets[OFFSET_CAMERA_STRUCT_COPY_FUNCTION] = (void *) 0x00406000;
            break;
        case Version_Polish:
            this->offsets[OFFSET_D3DCREATE] = (void *) 0x00402E30;
            this->offsets[OFFSET_IDIRECT3D9] = (void *) 0x00B929E0;
            this->offsets[OFFSET_DINPUT_COOPERATIVELEVEL] = (void *) 0x004232AF;
            this->offsets[OFFSET_DINPUTDEVICE_MOUSE] = (void *) 0x00B819D8;
            this->offsets[OFFSET_DINPUTDEVICE_KEYBOARD] = (void *) 0x00B819C8;
            this->offsets[OFFSET_WINDOW_HANDLE] = (void *) 0x00D1A600;
            this->offsets[OFFSET_WINDOW_X] = (void *) 0x00402321;
            this->offsets[OFFSET_WINDOW_Y] = (void *) 0x00402329;
            this->offsets[OFFSET_VIEWPORT_RESOLUTION_X] = (void *) 0x00B92B68;
            this->offsets[OFFSET_VIEWPORT_RESOLUTION_Y] = (void *) 0x00B92B6C;
            this->offsets[OFFSET_DEBUG_CONSOLE_DISABLED] = (void *) 0x00B929D8;
            this->offsets[OFFSET_CAMERA_POSITION] = (void *) 0xB830B4;
            this->offsets[OFFSET_PLAYER_POSITION] = (void *) 0xB81054;
            this->offsets[OFFSET_CAMERA_STRUCT_COPY_FUNCTION] = (void *) 0x45F2F0;
            break;
        case Version_Demo:
            this->offsets[OFFSET_D3DCREATE] = (void *) 0x004A0F30;
            this->offsets[OFFSET_IDIRECT3D9] = (void *) 0x00B31630;
            this->offsets[OFFSET_DINPUT_COOPERATIVELEVEL] = (void *) 0x0048AF96;
            this->offsets[OFFSET_DINPUTDEVICE_MOUSE] = (void *) 0x0098F248;
            this->offsets[OFFSET_DINPUTDEVICE_KEYBOARD] = (void *) 0x0098F238;
            this->offsets[OFFSET_WINDOW_HANDLE] = (void *) 0x009A76A0;
            this->offsets[OFFSET_WINDOW_X] = (void *) 0x004010E1;
            this->offsets[OFFSET_WINDOW_Y] = (void *) 0x004010E9;
            this->offsets[OFFSET_VIEWPORT_RESOLUTION_X] = (void *) 0x00B317B8;
            this->offsets[OFFSET_VIEWPORT_RESOLUTION_Y] = (void *) 0x00B317BC;
            this->offsets[OFFSET_DEBUG_CONSOLE_DISABLED] = (void *) 0x00990B40;
            this->offsets[OFFSET_CAMERA_POSITION] = (void *) 0x0098C514;
            this->offsets[OFFSET_PLAYER_POSITION] = (void *) 0x009901B4;
            this->offsets[OFFSET_CAMERA_STRUCT_COPY_FUNCTION] = (void *) 0x0040C3A0;
            break;
    }
}

void *Offsets::GetOffset(BGEOffset bgeOffset)
{
    if (this->offsets.count(bgeOffset) == 0)
    {
        cout << "Couldn't find " << magic_enum::enum_name(bgeOffset) << " for your BGE version "
             << magic_enum::enum_name(this->bgeVersion) << endl;

        FatalError();
        return nullptr;
    }

    return this->offsets[bgeOffset];
}