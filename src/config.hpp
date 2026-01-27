#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "key.hpp"
#include "control.hpp"

/* Modifier */
#define MOD_KEY MOD_ALT

/* Commands */
static const wchar_t* logout[]    = { L"shutdown",    L"/l", NULL };
static const wchar_t* terminal[]  = { L"wt.exe",      NULL };
static const wchar_t* wndmenu[]   = { L"wndmenu.exe", NULL };

/* Hotkey table */
const std::vector<Key> keys = {
    /* Modifier             Key         Function            Argument */
    {MOD_KEY,               'F',        float_window,       {}},
    {MOD_KEY,               'M',        maximize_window,    {}},
    {MOD_KEY,               'P',        execute,            wndmenu},
    {MOD_KEY,               '1',        focus_desktop,      0},
    {MOD_KEY,               '2',        focus_desktop,      1},
    {MOD_KEY,               '3',        focus_desktop,      2},
    {MOD_KEY,               '4',        focus_desktop,      3},
    {MOD_KEY,               '5',        focus_desktop,      4},
    {MOD_KEY,               '6',        focus_desktop,      5},
    {MOD_KEY,               '7',        focus_desktop,      6},
    {MOD_KEY,               '8',        focus_desktop,      7},
    {MOD_KEY,               '9',        focus_desktop,      8},
    {MOD_KEY | MOD_SHIFT,   '1',        send_desktop,       0},
    {MOD_KEY | MOD_SHIFT,   '2',        send_desktop,       1},
    {MOD_KEY | MOD_SHIFT,   '3',        send_desktop,       2},
    {MOD_KEY | MOD_SHIFT,   '4',        send_desktop,       3},
    {MOD_KEY | MOD_SHIFT,   '5',        send_desktop,       4},
    {MOD_KEY | MOD_SHIFT,   '6',        send_desktop,       5},
    {MOD_KEY | MOD_SHIFT,   '7',        send_desktop,       6},
    {MOD_KEY | MOD_SHIFT,   '8',        send_desktop,       7},
    {MOD_KEY | MOD_SHIFT,   '9',        send_desktop,       8},
    {MOD_KEY | MOD_SHIFT,   'C',        close_window,       {}},
    {MOD_KEY | MOD_SHIFT,   'Q',        execute,            logout},
    {MOD_KEY | MOD_SHIFT,   VK_RETURN,  execute,            terminal},
};

#endif