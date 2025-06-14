#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "key.hpp"
#include "control.hpp"

// Modifier macro
#define MOD_KEY MOD_ALT

// Hotkey table
const std::vector<Key> keys = {
    /*modifier              key         function            argument*/
    {MOD_KEY,               '1',        switchToDesktop,    0},
    {MOD_KEY,               '2',        switchToDesktop,    1},
    {MOD_KEY,               '3',        switchToDesktop,    2},
    {MOD_KEY,               '4',        switchToDesktop,    3},
    {MOD_KEY,               '5',        switchToDesktop,    4},
    {MOD_KEY,               '6',        switchToDesktop,    5},
    {MOD_KEY,               '7',        switchToDesktop,    6},
    {MOD_KEY,               '8',        switchToDesktop,    7},
    {MOD_KEY,               '9',        switchToDesktop,    8},
    {MOD_KEY,               'J',        log,                0},
    {MOD_KEY,               'K',        log,                0},
    {MOD_KEY,               'F',        log,                0},
    {MOD_KEY,               'M',        log,                0},
    {MOD_KEY | MOD_SHIFT,   'Q',        log,                0},
    {MOD_KEY | MOD_SHIFT,   'C',        log,                0},
    {MOD_KEY | MOD_SHIFT,   VK_RETURN,  spawn,              L"wt.exe"},
};

#endif CONFIG_HPP