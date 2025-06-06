#include <windows.h>
#include <iostream>

#define MOD_MASK MOD_ALT

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
    
    // Register keys
    RegisterHotKey(NULL, 1, MOD_MASK, '0');
    RegisterHotKey(NULL, 2, MOD_MASK, '1');
    RegisterHotKey(NULL, 3, MOD_MASK, '2');
    RegisterHotKey(NULL, 4, MOD_MASK, 'J');
    RegisterHotKey(NULL, 5, MOD_MASK, 'K');
    RegisterHotKey(NULL, 6, MOD_MASK | MOD_SHIFT, VK_RETURN);


    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_HOTKEY) {
            int id = static_cast<int>(msg.wParam);
            wchar_t buffer[256];
            swprintf_s(buffer, L"Pressed hotkey index %i", id);
            MessageBox(NULL, buffer, L"Success!", MB_OK);
        }
    }

    // Unregister keys
    UnregisterHotKey(NULL, 1);
    UnregisterHotKey(NULL, 2);
    UnregisterHotKey(NULL, 3);
    UnregisterHotKey(NULL, 4);
    UnregisterHotKey(NULL, 5);
    UnregisterHotKey(NULL, 6);

    return 0;
}

