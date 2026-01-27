#ifndef VIRTUAL_DESKTOP_HPP
#define VIRTUAL_DESKTOP_HPP

#include <combaseapi.h>
#include <shobjidl.h>
#include <shobjidl_core.h>
#include <wrl/client.h>

#include "interfaces.hpp"
#include "services.hpp"

inline Microsoft::WRL::ComPtr<IApplicationView> get_view(HWND window)
{
    Microsoft::WRL::ComPtr<IApplicationView> view;
    view_collection->GetViewForHwnd(window, &view);

    return view;
}

inline Microsoft::WRL::ComPtr<IObjectArray> get_desktops()
{
    Microsoft::WRL::ComPtr<IUnknown> desktops_unknown;
    desktop_manager_internal->GetDesktops(&desktops_unknown);
    Microsoft::WRL::ComPtr<IObjectArray> desktops;
    desktops_unknown.As(&desktops);

    return desktops;
}

inline Microsoft::WRL::ComPtr<IVirtualDesktop> get_desktop(IObjectArray* desktops, unsigned int index)
{
    Microsoft::WRL::ComPtr<IUnknown> desktop_unknown;
    desktops->GetAt(
        index,
        __uuidof(IUnknown),
        reinterpret_cast<void**>(desktop_unknown.GetAddressOf())
    );
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
    desktop_unknown.As(&desktop);

    return desktop;
}

inline Microsoft::WRL::ComPtr<IVirtualDesktop> get_desktop()
{
    Microsoft::WRL::ComPtr<IUnknown> desktop_unknown;
    desktop_manager_internal->GetCurrentDesktop(&desktop_unknown);
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
    desktop_unknown.As(&desktop);

    return desktop;
}

inline bool create_desktops(IObjectArray* desktops, unsigned int index)
{
    unsigned int count = 0;
    desktops->GetCount(&count);

    while (count <= index)
    {
        Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
        HRESULT hr = desktop_manager_internal->CreateDesktop(&desktop);
        if (FAILED(hr))
            return false;

        count++;
    }

    return true;
}

inline void move_view_to_desktop(IApplicationView* view, IVirtualDesktop* desktop)
{
    desktop_manager_internal->MoveViewToDesktop(
        view,
        desktop
    );
}

inline bool is_view_on_desktop(IApplicationView* view, IVirtualDesktop* desktop)
{
    if (!view || !desktop)
        return false;

    BOOL status = false;
    return SUCCEEDED(desktop->IsViewVisible(view, &status)) && status;
}

inline bool is_valid(HWND window)
{
    if (!IsWindow(window) || !IsWindowVisible(window))
        return false;

    HWND shell = GetShellWindow();
    if (window == shell)
        return false;

    LONG window_ex_style = GetWindowLong(window, GWL_EXSTYLE);
    if (window_ex_style & WS_EX_TOOLWINDOW)
        return false;

    LONG window_style = GetWindowLong(window, GWL_STYLE);
    if (!(window_style & WS_CAPTION))
        return false;

    return true;
}

inline bool can_view_move_desktop(IApplicationView* view)
{
    if (!view)
        return false;

    int status = 0;
    desktop_manager_internal->CanViewMoveDesktops(view, &status);
    return (status > 0);
}

inline void set_desktop(IVirtualDesktop* desktop)
{
    desktop_manager_internal->SwitchDesktop(desktop);
}

inline GUID get_desktop_id(IVirtualDesktop* desktop)
{
    GUID id = {};
    desktop->GetID(&id);
    return id;
}

inline GUID get_desktop_id()
{
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop = get_desktop();

    GUID id = {};
    desktop->GetID(&id);
    return id;
}

inline bool is_window_on_desktop(HWND window, const GUID& targetId)
{
    if (!IsWindow(window) || !desktop_manager)
        return false;

    GUID window_id = {};
    if (SUCCEEDED(desktop_manager->GetWindowDesktopId(window, &window_id)))
        return memcmp(&window_id, &targetId, sizeof(GUID)) == 0;

    BOOL on_current = FALSE;
    if (SUCCEEDED(desktop_manager->IsWindowOnCurrentVirtualDesktop(window, &on_current)) && on_current)
    {
        GUID currentId = get_desktop_id();
        return memcmp(&currentId, &targetId, sizeof(GUID)) == 0;
    }

    return false;
}

inline HWND get_next_window(IVirtualDesktop* desktop)
{
    HWND result = nullptr;
    const GUID target_id = get_desktop_id(desktop);

    EnumWindows([](HWND window, LPARAM lParam) -> BOOL {
        auto [desktop, result, targetId] = *reinterpret_cast<std::tuple<IVirtualDesktop*, HWND*, const GUID*>*>(lParam);

        if (!is_valid(window))
            return TRUE;

        if (!is_window_on_desktop(window, *targetId))
            return TRUE;

        Microsoft::WRL::ComPtr<IApplicationView> view = get_view(window);
        if (view && is_view_on_desktop(view.Get(), desktop))
        {
            *result = window;
            return FALSE;
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(&std::tuple{ desktop, &result, &target_id }));

    return result;
}

#endif