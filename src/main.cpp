#include <windows.h>
#include <vector>

#include "config.hpp"
#include "key.hpp"
#include "key_manager.hpp"
#include "services.hpp"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    initialize_services();

    KeyManager key_manager(1000);
    for (const Key& key : keys)
        key_manager.register_hotkey(key);

    key_manager.spin();

    CoUninitialize();

    return 0;
}