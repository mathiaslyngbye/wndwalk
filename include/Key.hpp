#pragma once

#include <functional>
#include <variant>

// Key argument
using Arg = std::variant<int, unsigned int, float, const wchar_t*>;

// Represents a single hotkey entry
struct Key
{
    unsigned int modifier;
    unsigned int symbol;
    std::function<void(const Arg&)> function;
    Arg arg;
};