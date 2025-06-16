#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <windows.h>

#include "log.hpp"
#include "focus.hpp"
#include "virtual_desktop.hpp"

inline void logArgument(const Arg &arg)
{
    int index = std::get<int>(arg);
    wchar_t msg[64];
    LOG("Called hotkey with index " << index);
}

inline void focusDesktop(const Arg &arg)
{
    const int index = std::get<int>(arg);

    // Declare source and destination
    const GUID fromDesktop  = getCurrentDesktopID();
    const HWND fromWindow   = GetForegroundWindow();
    const GUID toDesktop    = getDesktopID(index);
    const HWND toWindow     = decacheFocus(toDesktop);
    
    // Switch desktop 
    cacheFocus(fromDesktop, fromWindow);
    switchDesktop(index);
    setFocus(toWindow);
}

inline void sendDesktop(const Arg &arg)
{
    const int index = std::get<int>(arg);

    // Declare source and destination
    const HWND fromWindow   = GetForegroundWindow();
    const GUID toDesktop    = getDesktopID(index);
    
    // Send window to desktop and update cache
    moveViewToDesktop(fromWindow, index);
    cacheFocus(toDesktop, fromWindow);
}

inline void runCommand(const Arg& arg)
{
    // Get command
    const wchar_t* argCommandLine = std::get<const wchar_t*>(arg);
    std::wstring commandLine(argCommandLine);

    // Get user directory
    wchar_t userDirectory[MAX_PATH] = {};
    GetEnvironmentVariableW(L"USERPROFILE", userDirectory, MAX_PATH);

    // Get process info
    STARTUPINFOW si = {};
    PROCESS_INFORMATION pi = {};
    si.cb = sizeof(si);

    CreateProcessW(
        NULL,               // Application name (NULL = use command line)
        commandLine.data(), // Command line (mutable!)
        NULL,               // Process security attributes
        NULL,               // Thread security attributes
        FALSE,              // Inherit handles
        CREATE_NEW_CONSOLE, // Flags: new console, detached
        NULL,               // Use parent's environment
        userDirectory,      // Use parent's current directory
        &si,                // Startup info
        &pi                 // Process info
    );

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

#endif // CONTROL_HPP