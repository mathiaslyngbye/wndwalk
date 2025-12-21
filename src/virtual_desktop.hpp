#ifndef VIRTUAL_DESKTOP_HPP
#define VIRTUAL_DESKTOP_HPP

#include <combaseapi.h> // CoCreateInstance
#include <shobjidl.h>   // IServiceProvider, IObjectArray
#include <shobjidl_core.h>  // Contains IVirtualDesktopManager
#include <wrl/client.h> // Microsoft::WRL::ComPtr

#include "interfaces.hpp"
#include "services.hpp"

inline Microsoft::WRL::ComPtr<IApplicationView> getView(HWND window)
{
    Microsoft::WRL::ComPtr<IApplicationView> view;
    viewCollection->GetViewForHwnd(window, &view);

    return view;
}

inline Microsoft::WRL::ComPtr<IObjectArray> getDesktops()
{
    Microsoft::WRL::ComPtr<IUnknown> desktopsUnknown;
    desktopManagerInternal->GetDesktops(&desktopsUnknown);
    Microsoft::WRL::ComPtr<IObjectArray> desktops;
    desktopsUnknown.As(&desktops);

    return desktops;
}

inline Microsoft::WRL::ComPtr<IVirtualDesktop> getDesktop(IObjectArray* desktops, unsigned int index)
{
    Microsoft::WRL::ComPtr<IUnknown> desktopUnknown;
    desktops->GetAt(
        index,
        __uuidof(IUnknown),
        reinterpret_cast<void**>(desktopUnknown.GetAddressOf())
    );
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
    desktopUnknown.As(&desktop);

    return desktop;
}

inline Microsoft::WRL::ComPtr<IVirtualDesktop> getDesktop()
{
    Microsoft::WRL::ComPtr<IUnknown> desktopUnknown;
    desktopManagerInternal->GetCurrentDesktop(&desktopUnknown);
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
    desktopUnknown.As(&desktop);

    return desktop;
}

inline bool createDesktops(IObjectArray* desktops, unsigned int index)
{
    // Get desktop count
    unsigned int count = 0;
    desktops->GetCount(&count);

    // Create desktops
    while (count <= index)
    {
        Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
        HRESULT hr = desktopManagerInternal->CreateDesktop(&desktop);
        if (FAILED(hr))
            return false;

        count++;
    }
    return true;
}

inline void moveViewToDesktop(IApplicationView* view, IVirtualDesktop* desktop)
{
    // Move view to desktop
    desktopManagerInternal->MoveViewToDesktop(
        view,
        desktop
    );
}

inline bool isViewOnDesktop(IApplicationView* view, IVirtualDesktop* desktop)
{
    // Assert bad inputs
    if (!view || !desktop)
        return false;

    // Check if visible
    BOOL status = false;
    return SUCCEEDED(desktop->IsViewVisible(view, &status)) && status;
}

inline bool isValid(HWND window)
{
    // Assert invalid and hidden
    if (!IsWindow(window) || !IsWindowVisible(window))
        return false;

    // Assert shell
    HWND shell = GetShellWindow();
    if (window == shell)
        return false;

    // Assert tool window
    LONG windowExStyle = GetWindowLong(window, GWL_EXSTYLE);
    if (windowExStyle & WS_EX_TOOLWINDOW)
        return false;

    // Assert title window
    LONG windowStyle = GetWindowLong(window, GWL_STYLE);
    if (!(windowStyle & WS_CAPTION))
        return false;

    return true;
}

inline bool canViewMoveDesktop(IApplicationView* view)
{
    // Assert garbage view
    if (!view)
        return false;

    // Check if can move
    int status = 0;
    desktopManagerInternal->CanViewMoveDesktops(view, &status);
    return (status > 0);
}

inline void setDesktop(IVirtualDesktop* desktop)
{
    desktopManagerInternal->SwitchDesktop(desktop);
}

inline GUID getDesktopID(IVirtualDesktop* desktop)
{
    // Return desktop GUID
    GUID id = {};
    desktop->GetID(&id);
    return id;
}

inline GUID getDesktopID()
{
    // Get current desktop
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop = getDesktop();

    // Return desktop GUID
    GUID id = {};
    desktop->GetID(&id);
    return id;
}

inline bool isWindowOnDesktop(HWND window, const GUID& targetId)
{
    if (!IsWindow(window) || !desktopManager)
        return false;

    GUID windowId = {};
    if (SUCCEEDED(desktopManager->GetWindowDesktopId(window, &windowId)))
        return memcmp(&windowId, &targetId, sizeof(GUID)) == 0;

    // If target is current, allow the public API current-desktop check
    BOOL onCurrent = FALSE;
    if (SUCCEEDED(desktopManager->IsWindowOnCurrentVirtualDesktop(window, &onCurrent)) && onCurrent)
    {
        GUID currentId = getDesktopID();
        return memcmp(&currentId, &targetId, sizeof(GUID)) == 0;
    }

    return false;
}

inline HWND getNextWindow(IVirtualDesktop* desktop)
{
    HWND result = nullptr;
    const GUID targetId = getDesktopID(desktop);

    EnumWindows([](HWND window, LPARAM lParam) -> BOOL {
        auto [desktop, result, targetId] = *reinterpret_cast<std::tuple<IVirtualDesktop*, HWND*, const GUID*>*>(lParam);

        if (!isValid(window))
            return TRUE;

        // Ensure the raw HWND belongs to the target desktop before proceeding
        if (!isWindowOnDesktop(window, *targetId))
            return TRUE;

        Microsoft::WRL::ComPtr<IApplicationView> view = getView(window);
        if (view && isViewOnDesktop(view.Get(), desktop))
        {
            *result = window;
            return FALSE;
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(&std::tuple{ desktop, &result, &targetId }));

    return result;
}

#endif // VIRTUAL_DESKTOP_HPP