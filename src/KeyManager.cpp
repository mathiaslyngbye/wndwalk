 #include "KeyManager.hpp"
 
KeyManager::KeyManager(unsigned int baseId) : id(baseId) {}

KeyManager::~KeyManager() {
    for (const auto& [id, _] : callbacks) {
        UnregisterHotKey(NULL, id);
    }
    callbacks.clear();
}

void KeyManager::spin()
{
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        handleMessage(msg);
    }
}

bool KeyManager::registerHotKey(const Key& key)
{
    if (!RegisterHotKey(NULL, id, key.modifier, key.symbol)) {
        return false;
    }

    callbacks[id++] = key.function;
    return true;
}

bool KeyManager::handleMessage(const MSG& msg)
{
    // Assert hotkey message
    if (msg.message != WM_HOTKEY) 
        return false;
    
    // Find and run callback
    auto iterator = callbacks.find((unsigned int)msg.wParam);
    if (iterator != callbacks.end()) {
        iterator->second();
        return true;
    }

    return false;
}