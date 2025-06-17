#ifndef VIRTUAL_DESKTOP_HPP
#define VIRTUAL_DESKTOP_HPP

#include <combaseapi.h> // CoCreateInstance
#include <shobjidl.h>   // IServiceProvider, IObjectArray
#include <wrl/client.h> // Microsoft::WRL::ComPtr

#include "interfaces.hpp"
#include "services.hpp"

inline void moveViewToDesktop(HWND window, unsigned int index)
{
    // Assert image not background
    HWND shell = GetShellWindow();
    if (window == shell)
        return;
    
    // Get active view
    Microsoft::WRL::ComPtr<IApplicationView> view;
    viewCollection->GetViewForHwnd(window, &view);

    // Get desktops
    Microsoft::WRL::ComPtr<IUnknown> desktopsUnknown;
    desktopManager->GetDesktops(&desktopsUnknown);
    Microsoft::WRL::ComPtr<IObjectArray> desktops;
    desktopsUnknown.As(&desktops);

    // Create desktops if they dont exist
    unsigned int count = 0;
    desktops->GetCount(&count);
    while (count <= index)
    {
        Microsoft::WRL::ComPtr<IVirtualDesktop> desktopCreate;
        desktopManager->CreateDesktopW(&desktopCreate);
        count++;
    }

    // Get desktop from index
    Microsoft::WRL::ComPtr<IUnknown> desktopUnknown;
    desktops->GetAt(
        index,
        __uuidof(IUnknown),
        reinterpret_cast<void**>(desktopUnknown.GetAddressOf())
    );
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
    desktopUnknown.As(&desktop);

    // Move view to desktop
    desktopManager->MoveViewToDesktop(view.Get(), desktop.Get());
    SetForegroundWindow(shell);
}

inline void switchDesktop(unsigned int index)
{
    // Get desktops
    Microsoft::WRL::ComPtr<IUnknown> desktopsUnknown;
    desktopManager->GetDesktops(&desktopsUnknown);
    Microsoft::WRL::ComPtr<IObjectArray> desktops;
    desktopsUnknown.As(&desktops);

    // Create desktops if they dont exist
    unsigned int count = 0;
    desktops->GetCount(&count);
    while (count <= index)
    {
        Microsoft::WRL::ComPtr<IVirtualDesktop> desktopCreate;
        desktopManager->CreateDesktopW(&desktopCreate);
        count++;
    }

    // Get desktop at index
    Microsoft::WRL::ComPtr<IUnknown> desktopUnknown;
    desktops->GetAt(
        index, 
        __uuidof(IUnknown),
        reinterpret_cast<void**>(desktopUnknown.GetAddressOf())
    );

    // Switch to it
    desktopManager->SwitchDesktop(desktopUnknown.Get());
}

inline GUID getDesktopID(unsigned int index)
{
    // Get desktop array
    Microsoft::WRL::ComPtr<IUnknown> desktopsUnknown;
    desktopManager->GetDesktops(&desktopsUnknown);
    Microsoft::WRL::ComPtr<IObjectArray> desktops;
    desktopsUnknown.As(&desktops);

    // Get desktop count
    unsigned int count = 0;
    desktops->GetCount(&count);
    if (index >= count) 
        return GUID{};

    // Get desktop at index
    Microsoft::WRL::ComPtr<IUnknown> desktopUnknown;
    desktops->GetAt(
        index, 
        __uuidof(IUnknown),
        reinterpret_cast<void**>(desktopUnknown.GetAddressOf())
    );
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
    desktopUnknown.As(&desktop);

    // Return desktop GUID
    GUID id = {};
    desktop->GetID(&id);
    return id;
}

inline GUID getCurrentDesktopID()
{
    // Get current desktop
    Microsoft::WRL::ComPtr<IUnknown> desktopUnknown;
    desktopManager->GetCurrentDesktop(&desktopUnknown);
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
    desktopUnknown.As(&desktop);

    // Return desktop GUID
    GUID id = {};
    desktop->GetID(&id);
    return id;
}

#endif // VIRTUAL_DESKTOP_HPP