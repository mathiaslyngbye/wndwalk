
#include "Key.hpp"
#include "control.hpp"

// Modifier macro
#define MOD_KEY MOD_ALT

// Hotkey table
const std::vector<Key> keys = {
    /*modifier              key         function    argument*/
    {MOD_KEY,               '1',        action,     0},
    {MOD_KEY,               '2',        action,     1},
    {MOD_KEY,               '3',        action,     2},
    {MOD_KEY,               '4',        action,     3},
    {MOD_KEY,               '5',        action,     4},
    {MOD_KEY,               '6',        action,     5},
    {MOD_KEY,               '7',        action,     6},
    {MOD_KEY,               '8',        action,     7},
    {MOD_KEY,               '9',        action,     8},
    {MOD_KEY,               'J',        action,     0},
    {MOD_KEY,               'K',        action,     0},
    {MOD_KEY,               'F',        action,     0},
    {MOD_KEY,               'M',        action,     0},
    {MOD_KEY | MOD_SHIFT,   'Q',        action,     0},
    {MOD_KEY | MOD_SHIFT,   'C',        action,     0},
    {MOD_KEY | MOD_SHIFT,   VK_RETURN,  spawn,      L"wt.exe"},
};
