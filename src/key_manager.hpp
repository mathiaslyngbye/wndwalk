#ifndef KEY_MANAGER_HPP
#define KEY_MANAGER_HPP

#include <windows.h>
#include <unordered_map>
#include <functional>

#include "key.hpp"

class KeyManager
{
public:
    explicit KeyManager(unsigned int baseId = 1000);
    ~KeyManager();

    void spin();
    bool register_hotkey(const Key& key);

private:
    bool handle_message(const MSG& msg);

    unsigned int current_id_;
    std::unordered_map<unsigned int, std::pair<std::function<void(const Arg&)>, Arg>> callbacks_;
};

#endif