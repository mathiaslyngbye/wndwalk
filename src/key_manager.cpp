#include "key_manager.hpp"

KeyManager::KeyManager(unsigned int base_id) : current_id_(base_id) {}

KeyManager::~KeyManager()
{
    for (const auto& [id, _] : callbacks_)
    {
        UnregisterHotKey(NULL, id);
    }
    callbacks_.clear();
}

void KeyManager::spin()
{
    MSG msg = {};
    while (true)
    {
        int status = GetMessage(&msg, NULL, 0, 0);
        if (status == 0)
            break;
        if (status == -1)
            continue;

        handle_message(msg);
    }
}

bool KeyManager::register_hotkey(const Key& key)
{
    if (!RegisterHotKey(NULL, current_id_, key.modifier, key.symbol))
    {
        return false;
    }

    callbacks_[current_id_++] = {key.function, key.arg};

    return true;
}

bool KeyManager::handle_message(const MSG& msg)
{
    if (msg.message != WM_HOTKEY)
        return false;

    auto iterator = callbacks_.find(static_cast<unsigned int>(msg.wParam));
    if (iterator != callbacks_.end())
    {
        const auto& [function, argument] = iterator->second;
        function(argument);
        return true;
    }

    return false;
}
