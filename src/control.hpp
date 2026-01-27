#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <windows.h>
#include <shellapi.h>
#include <string>

#include "focus.hpp"
#include "virtual_desktop.hpp"

inline void close_window(const Arg &arg)
{
    HWND window = GetForegroundWindow();
    if (IsWindow(window))
        PostMessage(window, WM_CLOSE, 0, 0);
}

inline void maximize_window(const Arg& arg)
{
    HWND window = GetForegroundWindow();
    if (IsWindow(window))
        ShowWindow(window, SW_MAXIMIZE);
}

inline void float_window(const Arg& arg)
{
    HWND window = GetForegroundWindow();
    if (IsWindow(window))
        ShowWindow(window, SW_RESTORE);
}

inline void focus_desktop(const Arg &arg)
{
    const int index = std::get<int>(arg);

    Microsoft::WRL::ComPtr<IObjectArray> desktops = get_desktops();
    if (!create_desktops(desktops.Get(), index))
        return;
    desktops = get_desktops();

    Microsoft::WRL::ComPtr<IVirtualDesktop> from_desktop = get_desktop(desktops.Get(), index);
    const GUID from_desktop_id = get_desktop_id();
    const HWND from_window = GetForegroundWindow();
    Microsoft::WRL::ComPtr<IApplicationView> fromView = get_view(from_window);

    Microsoft::WRL::ComPtr<IVirtualDesktop> to_desktop = get_desktop(desktops.Get(), index);
    const GUID to_desktop_id = get_desktop_id(to_desktop.Get());
    const HWND to_window = decache_focus(to_desktop_id);
    Microsoft::WRL::ComPtr<IApplicationView> to_view = get_view(to_window);

    cache_focus(from_desktop_id, from_window);
    set_desktop(to_desktop.Get());

    if(to_window && to_view && is_view_on_desktop(to_view.Get(), to_desktop.Get()))
        set_focus(to_window);
}

inline void send_desktop(const Arg &arg)
{
    const int index = std::get<int>(arg);

    const HWND from_window = GetForegroundWindow();
    HWND shell = GetShellWindow();
    if (from_window == shell)
        return;

    Microsoft::WRL::ComPtr<IApplicationView> from_view = get_view(from_window);
    if (!can_view_move_desktop(from_view.Get()))
        return;

    Microsoft::WRL::ComPtr<IObjectArray> desktops = get_desktops();
    if (!create_desktops(desktops.Get(), index))
        return;
    desktops = get_desktops();

    Microsoft::WRL::ComPtr<IVirtualDesktop> from_desktop = get_desktop();
    if (!is_view_on_desktop(from_view.Get(), from_desktop.Get()))
        return;

    Microsoft::WRL::ComPtr<IVirtualDesktop> to_desktop = get_desktop(desktops.Get(), index);
    const GUID to_desktop_id = get_desktop_id(to_desktop.Get());

    move_view_to_desktop(from_view.Get(), to_desktop.Get());
    cache_focus(to_desktop_id, from_window);

    HWND to_window = get_next_window(from_desktop.Get());
    set_focus(to_window);
}

inline void execute(const Arg& arg)
{
    const auto command = std::get<const wchar_t**>(arg);

    if (!command || !command[0])
        return;

    const wchar_t* exe = command[0];
    std::wstring parameters;
    for (size_t i = 1; command[i]; i++)
    {
        if (!parameters.empty())
            parameters.push_back(L' ');

        parameters += command[i];
    }

    ShellExecuteW(
        nullptr,
        L"open",
        exe,
        parameters.empty() ? nullptr : parameters.c_str(),
        nullptr,
        SW_SHOWNORMAL
    );
}

#endif