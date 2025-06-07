#include <windows.h>

#include "Key.hpp"

// Modifier macro
#define MOD_KEY MOD_ALT

// Action function
void action() {
    MessageBox(NULL, L"Hotkey pressed!", L"Success!", MB_OK);
};

// Hotkey table
const std::vector<Key> keys = {
    /*modifier              key         function*/
    {MOD_KEY,               '1',        action},
    {MOD_KEY,               '2',        action},
    {MOD_KEY,               '3',        action},
    {MOD_KEY,               '4',        action},
    {MOD_KEY,               '5',        action},
    {MOD_KEY,               '6',        action},
    {MOD_KEY,               '7',        action},
    {MOD_KEY,               '8',        action},
    {MOD_KEY,               '9',        action},
    {MOD_KEY,               'J',        action},
    {MOD_KEY,               'K',        action},
    {MOD_KEY,               'F',        action},
    {MOD_KEY,               'M',        action},
    {MOD_KEY | MOD_SHIFT,   'Q',        action},
    {MOD_KEY | MOD_SHIFT,   'C',        action},
    {MOD_KEY | MOD_SHIFT,   VK_RETURN,  action},
};
