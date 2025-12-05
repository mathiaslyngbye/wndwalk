#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <windows.h>
#include <string>

#include "focus.hpp"
#include "virtual_desktop.hpp"

inline void closeWindow(const Arg &arg)
{
    HWND window = GetForegroundWindow();
    if (IsWindow(window))
        PostMessage(window, WM_CLOSE, 0, 0);
}

inline void maximizeWindow(const Arg& arg)
{
    HWND window = GetForegroundWindow();
    if (IsWindow(window))
        ShowWindow(window, SW_MAXIMIZE);
}

inline void floatWindow(const Arg& arg)
{
    HWND window = GetForegroundWindow();
    if (IsWindow(window))
        ShowWindow(window, SW_RESTORE);
}

inline void focusDesktop(const Arg &arg)
{
    // Get index argument
    const int index = std::get<int>(arg);
    
    // Get desktops and create more if needed
    Microsoft::WRL::ComPtr<IObjectArray> desktops = getDesktops();
    if (!createDesktops(desktops.Get(), index))
        return;
    desktops = getDesktops();

    // Get source
    Microsoft::WRL::ComPtr<IVirtualDesktop> fromDesktop = getDesktop(desktops.Get(), index);
    const GUID fromDesktopID = getDesktopID();
    const HWND fromWindow = GetForegroundWindow();
    Microsoft::WRL::ComPtr<IApplicationView> fromView = getView(fromWindow);
    
    // Get destination
    Microsoft::WRL::ComPtr<IVirtualDesktop> toDesktop = getDesktop(desktops.Get(), index);
    const GUID toDesktopID = getDesktopID(toDesktop.Get());
    const HWND toWindow = decacheFocus(toDesktopID);
    Microsoft::WRL::ComPtr<IApplicationView> toView = getView(toWindow);
    
    // Verify source and cache
    cacheFocus(fromDesktopID, fromWindow);

    // Switch desktop 
    setDesktop(toDesktop.Get());

    // Verify destination and go
    if(toWindow && toView && isViewOnDesktop(toView.Get(), toDesktop.Get()))
        setFocus(toWindow);
}

inline void sendDesktop(const Arg &arg)
{
    // Get index argument
    const int index = std::get<int>(arg);
    
    // Get and assert window
    const HWND fromWindow = GetForegroundWindow();
    HWND shell = GetShellWindow();
    if (fromWindow == shell)
        return;
    
    // Get and assert source view
    Microsoft::WRL::ComPtr<IApplicationView> fromView = getView(fromWindow);
    if (!canViewMoveDesktop(fromView.Get()))
        return;
    
    // Get desktops and create more if needed
    Microsoft::WRL::ComPtr<IObjectArray> desktops = getDesktops();
    if (!createDesktops(desktops.Get(), index))
        return;
    desktops = getDesktops();

    // Get and assert source desktop
    Microsoft::WRL::ComPtr<IVirtualDesktop> fromDesktop = getDesktop();
    if (!isViewOnDesktop(fromView.Get(), fromDesktop.Get()))
        return;

    // Get destination
    Microsoft::WRL::ComPtr<IVirtualDesktop> toDesktop = getDesktop(desktops.Get(), index);
    const GUID toDesktopID = getDesktopID(toDesktop.Get());
    
    // Send window to desktop and update cache
    moveViewToDesktop(fromView.Get(), toDesktop.Get());
    cacheFocus(toDesktopID, fromWindow);

    // Select next focus
    HWND toWindow = getNextWindow(fromDesktop.Get());
    setFocus(toWindow);
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