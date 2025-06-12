#pragma once

#include <Windows.h>
#include <shobjidl.h>
#include <wrl/client.h>
#include <stdexcept>
#include <combaseapi.h>
#include <comdef.h>
#include <iostream>

#include "interfaces.hpp"

using namespace Microsoft::WRL;

class VirtualDesktop
{
public:
    VirtualDesktop()
    {
        CoInitializeEx(nullptr, COINIT_MULTITHREADED);

        ComPtr<IServiceProvider> provider;
        CoCreateInstance(CLSID_ImmersiveShell, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&provider));
        
        provider->QueryService(
            CLSID_VirtualDesktopManagerInternal,
            IID_IVirtualDesktopManagerInternal,
            reinterpret_cast<void**>(vdmInternal.GetAddressOf())
        );
    }

    void switchToDesktopByIndex(UINT index)
    {
        // Get desktops as unknown
        ComPtr<IUnknown> unknown;
        vdmInternal->GetDesktops(&unknown);
        
        // Get desktops from unknown
        ComPtr<IObjectArray> desktops;
        unknown.As(&desktops);

        // Get desktop count
        UINT count = 0;
        desktops->GetCount(&count);
        if (index >= count) throw std::out_of_range("Invalid desktop index");

        // Get desktop at index
        ComPtr<IUnknown> desktop;
        desktops->GetAt(index, __uuidof(IUnknown), reinterpret_cast<void**>(desktop.GetAddressOf()));

        // Switch to it
        vdmInternal->SwitchDesktop(desktop.Get());
    }

private:
    ComPtr<IVirtualDesktopManagerInternal> vdmInternal;
};
