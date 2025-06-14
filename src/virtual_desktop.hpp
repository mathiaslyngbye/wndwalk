#pragma once

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
        IID_IVirtualDesktopManagerInternal,
        reinterpret_cast<void**>(vdmInternal.GetAddressOf())
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
    desktops->GetAt(index, __uuidof(IUnknown), reinterpret_cast<void**>(desktop.GetAddressOf()));

    // Switch to it
    vdmInternal->SwitchDesktop(desktop.Get());
}
