#ifndef VIRTUAL_DESKTOP_HPP
#define VIRTUAL_DESKTOP_HPP

#include <combaseapi.h> // CoCreateInstance
#include <shobjidl.h>   // IServiceProvider, IObjectArray
#include <wrl/client.h> // Microsoft::WRL::ComPtr

#include "interfaces.hpp"

inline Microsoft::WRL::ComPtr<IVirtualDesktopManagerInternal> getVirtualDesktopManagerInternal()
{
    Microsoft::WRL::ComPtr<IServiceProvider> provider;
    CoCreateInstance(
        CLSID_ImmersiveShell,
        nullptr,
        CLSCTX_ALL,
        IID_PPV_ARGS(&provider)
    );

    Microsoft::WRL::ComPtr<IVirtualDesktopManagerInternal> vdmInternal;
    provider->QueryService(
        CLSID_VirtualDesktopManagerInternal,
        vdmInternal.GetAddressOf()
    );

    return vdmInternal;
}

inline Microsoft::WRL::ComPtr<IApplicationViewCollection> getApplicationViewCollection()
{
    Microsoft::WRL::ComPtr<IServiceProvider> provider;
    CoCreateInstance(
        CLSID_ImmersiveShell,
        nullptr,
        CLSCTX_ALL,
        IID_PPV_ARGS(&provider)
    );

    Microsoft::WRL::ComPtr<IApplicationViewCollection> collection;
    provider->QueryService(
        __uuidof(IApplicationViewCollection),
        collection.GetAddressOf()
    );

    return collection;
}

inline void moveViewToDesktop(HWND window, unsigned int index)
{
    // Get interfaces
    Microsoft::WRL::ComPtr<IApplicationViewCollection> viewCollection   = getApplicationViewCollection();
    Microsoft::WRL::ComPtr<IVirtualDesktopManagerInternal> vdmInternal  = getVirtualDesktopManagerInternal();

    // Get active view
    Microsoft::WRL::ComPtr<IApplicationView> view;
    viewCollection->GetViewForHwnd(window, &view);

    // Get desktops
    Microsoft::WRL::ComPtr<IUnknown> desktopsUnknown;
    vdmInternal->GetDesktops(&desktopsUnknown);
    Microsoft::WRL::ComPtr<IObjectArray> desktops;
    desktopsUnknown.As(&desktops);

    // Create desktops if they dont exist
    unsigned int count = 0;
    desktops->GetCount(&count);
    while (count <= index)
    {
        Microsoft::WRL::ComPtr<IVirtualDesktop> desktopCreate;
        vdmInternal->CreateDesktopW(&desktopCreate);
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
    vdmInternal->MoveViewToDesktop(view.Get(), desktop.Get());

    // Try focusing the shell window
    HWND shell = GetShellWindow();
    if (IsWindow(shell))
        SetForegroundWindow(shell);
}

inline void switchDesktop(unsigned int index)
{
    // Get vdm internal
    Microsoft::WRL::ComPtr<IVirtualDesktopManagerInternal> vdmInternal = getVirtualDesktopManagerInternal();

    // Get desktops
    Microsoft::WRL::ComPtr<IUnknown> desktopsUnknown;
    vdmInternal->GetDesktops(&desktopsUnknown);
    Microsoft::WRL::ComPtr<IObjectArray> desktops;
    desktopsUnknown.As(&desktops);

    // Create desktops if they dont exist
    unsigned int count = 0;
    desktops->GetCount(&count);
    while (count <= index)
    {
        Microsoft::WRL::ComPtr<IVirtualDesktop> desktopCreate;
        vdmInternal->CreateDesktopW(&desktopCreate);
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
    vdmInternal->SwitchDesktop(desktopUnknown.Get());
}

inline GUID getDesktopID(unsigned int index)
{
    // Get vdm internal
    Microsoft::WRL::ComPtr<IVirtualDesktopManagerInternal> vdmInternal = getVirtualDesktopManagerInternal();

    // Get desktop array
    Microsoft::WRL::ComPtr<IUnknown> desktopsUnknown;
    vdmInternal->GetDesktops(&desktopsUnknown);
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
    // Get vdm internal
    Microsoft::WRL::ComPtr<IVirtualDesktopManagerInternal> vdmInternal = getVirtualDesktopManagerInternal();
    
    // Get current desktop
    Microsoft::WRL::ComPtr<IUnknown> desktopUnknown;
    vdmInternal->GetCurrentDesktop(&desktopUnknown);
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
    desktopUnknown.As(&desktop);

    // Return desktop GUID
    GUID id = {};
    desktop->GetID(&id);
    return id;
}

#endif // VIRTUAL_DESKTOP_HPP