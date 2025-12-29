#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <windows.h>
#include <shellapi.h>
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
    // Get command from arg
    const auto command = std::get<const wchar_t**>(arg);

    // Assert valid input
    if (!command || !command[0])
        return;

    // Construct command and argument variables
    const wchar_t* exe = command[0];
    std::wstring parameters;
    for (size_t i = 1; command[i]; i++)
    {
        if (!parameters.empty())
            parameters.push_back(L' ');

        parameters += command[i];
    }

    // Run command
    ShellExecuteW(
        nullptr,
        L"open",
        exe,
        parameters.empty() ? nullptr : parameters.c_str(),
        nullptr,
        SW_SHOWNORMAL
    );
}


#endif // CONTROL_HPP