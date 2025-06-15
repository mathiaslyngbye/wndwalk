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

inline void switchDesktop(unsigned int index)
{
    // Get vdm internal
    Microsoft::WRL::ComPtr<IVirtualDesktopManagerInternal> vdmInternal = getVirtualDesktopManagerInternal();

    // Get desktops as unknown
    Microsoft::WRL::ComPtr<IUnknown> unknown;
    vdmInternal->GetDesktops(&unknown);
        
    // Get desktops from unknown
    Microsoft::WRL::ComPtr<IObjectArray> desktops;
    unknown.As(&desktops);

    // Get desktop count
    unsigned int count = 0;
    desktops->GetCount(&count);
    if (index >= count) 
        return;

    // Get desktop at index
    Microsoft::WRL::ComPtr<IUnknown> desktop;
    desktops->GetAt(
        index, 
        __uuidof(IUnknown),
        reinterpret_cast<void**>(desktop.GetAddressOf())
    );

    // Switch to it
    vdmInternal->SwitchDesktop(desktop.Get());
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