#include <windows.h>
#include <vector>

#include "config.hpp"
#include "key.hpp"
#include "key_manager.hpp"
#include "services.hpp"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
    // Initialize use of com interfaces in thread
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    // Initialize services used by control functions
    initializeServices();

    // Register keys
    KeyManager keyManager(1000);
    for (const Key& key : keys)
        keyManager.registerHotKey(key);

    // Main message loop
    keyManager.spin();

    CoUninitialize();

    return 0;
}