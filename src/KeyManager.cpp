#include "KeyManager.hpp"

KeyManager::KeyManager(unsigned int baseId) : currentId(baseId) {}

KeyManager::~KeyManager()
{
    for (const auto& [id, _] : callbacks)
    {
        UnregisterHotKey(NULL, id);
    }
    callbacks.clear();
}

void KeyManager::spin()
{
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        handleMessage(msg);
    }
}

bool KeyManager::registerHotKey(const Key& key)
{
    if (!RegisterHotKey(NULL, currentId, key.modifier, key.symbol))
    {
        return false;
    }

    callbacks[currentId++] = {key.function, key.arg};
    return true;
}

bool KeyManager::handleMessage(const MSG& msg)
{
    if (msg.message != WM_HOTKEY)
        return false;

    auto iterator = callbacks.find(static_cast<unsigned int>(msg.wParam));
    if (iterator != callbacks.end())
    {
        const auto& [function, argument] = iterator->second;
        function(argument);
        return true;
    }

    return false;
}
