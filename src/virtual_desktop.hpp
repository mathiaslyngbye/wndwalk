#ifndef VIRTUAL_DESKTOP_HPP
#define VIRTUAL_DESKTOP_HPP

#include <combaseapi.h> // CoCreateInstance
#include <shobjidl.h>   // IServiceProvider, IObjectArray
#include <wrl/client.h> // Microsoft::WRL::ComPtr

#include "interfaces.hpp"
#include "services.hpp"

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

inline Microsoft::WRL::ComPtr<IApplicationView> getView(HWND window)
{
    Microsoft::WRL::ComPtr<IApplicationView> view;
    viewCollection->GetViewForHwnd(window, &view);

    return view;
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

inline void moveViewToDesktop(HWND window, unsigned int index)
{
    // Assert image not background
    HWND shell = GetShellWindow();
    if (window == shell)
        return;

    // Get desktops and create if missing
    Microsoft::WRL::ComPtr<IObjectArray> desktops = getDesktops();
    createDesktops(desktops.Get(), index);
    
    // Get desktop and view
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop = getDesktop(desktops.Get(), index);
    Microsoft::WRL::ComPtr<IApplicationView> view   = getView(window);

    // Move view to desktop
    desktopManager->MoveViewToDesktop(
        view.Get(),
        desktop.Get()
    );
    
    // Select next focus
    SetForegroundWindow(shell);
}

inline void switchDesktop(unsigned int index)
{
    // Get desktops and create if missing
    Microsoft::WRL::ComPtr<IObjectArray> desktops = getDesktops();
    createDesktops(desktops.Get(), index);

    // Get desktop at index
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop = getDesktop(desktops.Get(), index);

    // Switch to it
    desktopManager->SwitchDesktop(desktop.Get());
}

inline GUID getDesktopID(unsigned int index)
{
    // Get desktops and create if missing
    Microsoft::WRL::ComPtr<IObjectArray> desktops = getDesktops();
    createDesktops(desktops.Get(), index);

    // Get desktop at index
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop = getDesktop(desktops.Get(), index);

    // Return desktop GUID
    GUID id = {};
    desktop->GetID(&id);
    return id;
}

inline GUID getCurrentDesktopID()
{
    // Get current desktop
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop = getDesktop();

    // Return desktop GUID
    GUID id = {};
    desktop->GetID(&id);
    return id;
}

#endif // VIRTUAL_DESKTOP_HPP