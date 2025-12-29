#ifndef SERVICES_HPP
#define SERVICES_HPP

#include <wrl/client.h>
#include "interfaces.hpp"

inline Microsoft::WRL::ComPtr<IVirtualDesktopManager> desktopManager;
inline Microsoft::WRL::ComPtr<IVirtualDesktopManagerInternal> desktopManagerInternal;
inline Microsoft::WRL::ComPtr<IApplicationViewCollection> viewCollection;

inline void initializeServices()
{
    // Public desktop manager
    CoCreateInstance(
        CLSID_VirtualDesktopManager,
        nullptr,
        CLSCTX_ALL,
        IID_PPV_ARGS(&desktopManager)
    );

    // Immersive shell + advertised services
    Microsoft::WRL::ComPtr<IServiceProvider> serviceProvider;
    CoCreateInstance(
        CLSID_ImmersiveShell,
        nullptr,
        CLSCTX_ALL,
        IID_PPV_ARGS(&serviceProvider)
    );

    serviceProvider->QueryService(
        __uuidof(IApplicationViewCollection),
        IID_PPV_ARGS(&viewCollection)
    );

    serviceProvider->QueryService(
        CLSID_VirtualDesktopManagerInternal,
        IID_PPV_ARGS(&desktopManagerInternal)
    );
};

#endif // SERVICES_HPP