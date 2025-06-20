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
    desktopManager->GetDesktops(&desktopsUnknown);
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
    desktopManager->GetCurrentDesktop(&desktopUnknown);
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
    desktopUnknown.As(&desktop);

    return desktop;
}

inline void createDesktops(IObjectArray* desktops, unsigned int index)
{
    // Get desktop count
    unsigned int count = 0;
    desktops->GetCount(&count);

    // Create desktops
    while (count <= index)
    {
        Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
        desktopManager->CreateDesktopW(&desktop);
        count++;
    }
}

inline void moveViewToDesktop(IApplicationView* view, IVirtualDesktop* desktop)
{
    // Move view to desktop
    desktopManager->MoveViewToDesktop(
        view,
        desktop
    );
}

inline bool canViewMoveDesktop(IApplicationView* view)
{
    // Assert garbage view
    if (!view)
        return false;

    // Move view to desktop
    int status = 0;
    desktopManager->CanViewMoveDesktops(
        view,
        &status
    );

    // Return boolean result
    return (status > 0);
}

inline void setDesktop(IVirtualDesktop* desktop)
{
    desktopManager->SwitchDesktop(desktop);
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

#endif // VIRTUAL_DESKTOP_HPP