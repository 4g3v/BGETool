#pragma once

#include <Windows.h>
#include <vector>
//Thanks to https://guidedhacking.com

namespace mem
{
    void Patch(BYTE *dst, BYTE *src, unsigned int size);

    void PatchByte(BYTE *dst, BYTE b);

    void PatchInt(int *dst, int b);

    void PatchEx(BYTE *dst, BYTE *src, unsigned int size, HANDLE hProcess);

    void Nop(BYTE *dst, unsigned int size);

    void NopEx(BYTE *dst, unsigned int size, HANDLE hProcess);

    uintptr_t FindDMAAddy(uintptr_t ptr, std::vector<unsigned int> offsets);
}