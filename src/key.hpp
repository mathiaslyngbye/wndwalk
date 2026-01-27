#ifndef KEY_HPP
#define KEY_HPP

#include <functional>
#include <variant>

using Arg = std::variant<int, unsigned int, float, const wchar_t**>;

struct Key
{
    unsigned int modifier;
    unsigned int symbol;
    std::function<void(const Arg&)> function;
    Arg arg;
};

#endif