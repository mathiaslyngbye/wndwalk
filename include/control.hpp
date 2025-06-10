#pragma once

#include <windows.h>

inline void action(const Arg &arg)
{
    int index = std::get<int>(arg);
    wchar_t msg[64];
    swprintf_s(msg, L"Hotkey %d pressed!", index);
    MessageBox(NULL, msg, L"Hotkey Triggered", MB_OK);
}

inline void spawn(const Arg& arg)
{
    // Get command
    const wchar_t* argCommand = std::get<const wchar_t*>(arg);
    std::wstring command(argCommand);

    // Get user directory
    wchar_t userDir[MAX_PATH] = {};
    GetEnvironmentVariableW(L"USERPROFILE", userDir, MAX_PATH);

    // Get process info
    STARTUPINFOW si = {};
    PROCESS_INFORMATION pi = {};
    si.cb = sizeof(si);

    CreateProcessW(
        NULL,               // Application name (NULL = use command line)
        command.data(),     // Command line (mutable!)
        NULL,               // Process security attributes
        NULL,               // Thread security attributes
        FALSE,              // Inherit handles
        CREATE_NEW_CONSOLE, // Flags: new console, detached
        NULL,               // Use parent's environment
        userDir,            // Use parent's current directory
        &si,                // Startup info
        &pi                 // Process info
    );

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}