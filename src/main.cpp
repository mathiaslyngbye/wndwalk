#include <windows.h>
#include <iostream>
#include <vector>
#include <functional>

#include "config.hpp"
#include "key.hpp"
#include "key_manager.hpp"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) 
{
    // Create key manager
    KeyManager keyManager(1000);

    // Register all hotkeys
    for (const Key& key : keys)
        keyManager.registerHotKey(key);

    // Main message loop
    keyManager.spin();

    return 0;
}