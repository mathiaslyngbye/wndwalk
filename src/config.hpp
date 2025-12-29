#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "key.hpp"
#include "control.hpp"

// Modifier macro
#define MOD_KEY MOD_ALT

// Commands
static const wchar_t* logout[]    = { L"shutdown",    L"/l", NULL };
static const wchar_t* terminal[]  = { L"wt.exe",      NULL };
static const wchar_t* wndmenu[]   = { L"wndmenu.exe", NULL };

// Hotkey table
const std::vector<Key> keys = {
    /*modifier              key         function        argument*/
    {MOD_KEY,               'F',        floatWindow,    {}},
    {MOD_KEY,               'M',        maximizeWindow, {}},
    {MOD_KEY,               'P',        execute,        wndmenu},
    {MOD_KEY,               '1',        focusDesktop,   0},
    {MOD_KEY,               '2',        focusDesktop,   1},
    {MOD_KEY,               '3',        focusDesktop,   2},
    {MOD_KEY,               '4',        focusDesktop,   3},
    {MOD_KEY,               '5',        focusDesktop,   4},
    {MOD_KEY,               '6',        focusDesktop,   5},
    {MOD_KEY,               '7',        focusDesktop,   6},
    {MOD_KEY,               '8',        focusDesktop,   7},
    {MOD_KEY,               '9',        focusDesktop,   8},
    {MOD_KEY | MOD_SHIFT,   '1',        sendDesktop,    0},
    {MOD_KEY | MOD_SHIFT,   '2',        sendDesktop,    1},
    {MOD_KEY | MOD_SHIFT,   '3',        sendDesktop,    2},
    {MOD_KEY | MOD_SHIFT,   '4',        sendDesktop,    3},
    {MOD_KEY | MOD_SHIFT,   '5',        sendDesktop,    4},
    {MOD_KEY | MOD_SHIFT,   '6',        sendDesktop,    5},
    {MOD_KEY | MOD_SHIFT,   '7',        sendDesktop,    6},
    {MOD_KEY | MOD_SHIFT,   '8',        sendDesktop,    7},
    {MOD_KEY | MOD_SHIFT,   '9',        sendDesktop,    8},
    {MOD_KEY | MOD_SHIFT,   'C',        closeWindow,    {}},
    {MOD_KEY | MOD_SHIFT,   'Q',        execute,        logout},
    {MOD_KEY | MOD_SHIFT,   VK_RETURN,  execute,        terminal},
};

#endif // CONFIG_HPP