#pragma once

#include <functional>

// Represents a single hotkey entry
struct Key
{
    unsigned int modifier;
    unsigned int symbol;
    std::function<void()> function;
};