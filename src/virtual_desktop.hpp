#ifndef VIRTUAL_DESKTOP_HPP
#define VIRTUAL_DESKTOP_HPP

#include <combaseapi.h> // CoCreateInstance
#include <shobjidl.h>   // IServiceProvider, IObjectArray
#include <wrl/client.h> // Microsoft::WRL::ComPtr

#include "focus.hpp"
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

inline void switchToDesktopByIndex(unsigned int index)
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

inline GUID getCurrentDesktopId()
{
    // Get vdm internal
    Microsoft::WRL::ComPtr<IVirtualDesktopManagerInternal> vdmInternal = getVirtualDesktopManagerInternal();
    
    // Get current desktop as unknown
    Microsoft::WRL::ComPtr<IUnknown> unknown;
    vdmInternal->GetCurrentDesktop(&unknown);

    // Get desktop from unknown
    Microsoft::WRL::ComPtr<IVirtualDesktop> desktop;
    unknown.As(&desktop);

    GUID id = {};
    desktop->GetID(&id);
    return id;
}

#endif // VIRTUAL_DESKTOP_HPP