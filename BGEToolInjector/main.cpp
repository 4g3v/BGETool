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
#include <string>
#include <Windows.h>

using namespace std;

int main(int argc, char **argv)
{
    cout << "BGEToolInjector" << endl << endl;

    string bgeName = "BGE.exe";
    if (argc == 2)
        bgeName = argv[1];

    STARTUPINFO startupInfo = {sizeof(startupInfo)};
    PROCESS_INFORMATION processInformation;

    if (!CreateProcessA(bgeName.c_str(), nullptr, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr,
                        &startupInfo, &processInformation))
    {
        cout << "Failed to create " << bgeName << "!" << endl;
        return -1;
    }
    HANDLE bgeHandle = processInformation.hProcess;
    cout << "BGE handle: " << bgeHandle << endl;

    auto loadLibrary = (LPVOID) GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");
    cout << "LoadLibraryA is at " << loadLibrary << endl;

    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    string bgeToolPath = string(buffer);
    bgeToolPath = bgeToolPath.substr(0, bgeToolPath.find_last_of('\\')).append("\\BGETool.dll");

    char *bgeToolPathC = (char *) bgeToolPath.c_str();
    SIZE_T bgeToolPathLen = strlen(bgeToolPathC);

    auto loadLibraryArg = (LPVOID) VirtualAllocEx(bgeHandle, nullptr, bgeToolPathLen, MEM_RESERVE | MEM_COMMIT,
                                                  PAGE_READWRITE);
    if (!loadLibraryArg)
    {
        cout << "Failed to allocate memory inside BGE!" << endl;
        return -1;
    }

    cout << "Allocated memory in BGE at: " << loadLibraryArg << endl;

    SIZE_T writtenBytes = 0;
    if (!WriteProcessMemory(bgeHandle, loadLibraryArg, bgeToolPathC, bgeToolPathLen, &writtenBytes))
    {
        cout << "Failed to write to the newly allocated memory!";
        return -1;
    }

    cout << "Wrote " << writtenBytes << " bytes to it." << endl;

    HANDLE bgeToolThread = CreateRemoteThread(bgeHandle, nullptr, 0, (LPTHREAD_START_ROUTINE) loadLibrary,
                                              loadLibraryArg, CREATE_SUSPENDED, nullptr);
    if (!bgeToolThread)
    {
        cout << "Failed to create remote thread in BGE!" << endl;
        return -1;
    }

    cout << "Created LoadLibrary RemoteThread!" << endl;

    CloseHandle(bgeHandle);
    ResumeThread(bgeToolThread);

    return 0;
}