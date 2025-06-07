#pragma once

#include <windows.h>
#include <unordered_map>
#include <functional>
#include <vector>

#include "Key.hpp"

class KeyManager 
{
public:
    explicit KeyManager(unsigned int baseId = 1000);
    ~KeyManager();
    
    void spin();
    bool registerHotKey(const Key& key);

private:
    bool handleMessage(const MSG& msg);

    unsigned int currentId;
    std::unordered_map<unsigned int, std::pair<std::function<void(const Arg&)>, Arg>> callbacks;
};
