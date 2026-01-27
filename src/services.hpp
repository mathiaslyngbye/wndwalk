#ifndef SERVICES_HPP
#define SERVICES_HPP

#include <wrl/client.h>

#include "interfaces.hpp"

inline Microsoft::WRL::ComPtr<IVirtualDesktopManager> desktop_manager;
inline Microsoft::WRL::ComPtr<IVirtualDesktopManagerInternal> desktop_manager_internal;
inline Microsoft::WRL::ComPtr<IApplicationViewCollection> view_collection;

inline void initialize_services()
{
    CoCreateInstance(
        CLSID_VirtualDesktopManager,
        nullptr,
        CLSCTX_ALL,
        IID_PPV_ARGS(&desktop_manager)
    );

    Microsoft::WRL::ComPtr<IServiceProvider> service_provider;
    CoCreateInstance(
        CLSID_ImmersiveShell,
        nullptr,
        CLSCTX_ALL,
        IID_PPV_ARGS(&service_provider)
    );

    service_provider->QueryService(
        __uuidof(IApplicationViewCollection),
        IID_PPV_ARGS(&view_collection)
    );

    service_provider->QueryService(
        CLSID_VirtualDesktopManagerInternal,
        IID_PPV_ARGS(&desktop_manager_internal)
    );
};

#endif