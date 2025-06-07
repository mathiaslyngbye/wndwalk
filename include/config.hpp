#include <windows.h>

#include "Key.hpp"

// Modifier macro
#define MOD_KEY MOD_ALT

void action(const Arg &arg) {
    int index = std::get<int>(arg);
    wchar_t msg[64];
    swprintf_s(msg, L"Hotkey %d pressed!", index);
    MessageBox(NULL, msg, L"Hotkey Triggered", MB_OK);
}

void spawn(const Arg& arg)
{
    // Get command
    const wchar_t* argCommand = std::get<const wchar_t*>(arg);
    std::wstring command(argCommand);

    // Get user directory
    wchar_t userDir[MAX_PATH] = {};
    GetEnvironmentVariableW(L"USERPROFILE", userDir, MAX_PATH);

    // Get process info
    STARTUPINFOW si = {};
    PROCESS_INFORMATION pi = {};
    si.cb = sizeof(si);

    CreateProcessW(
        NULL,               // Application name (NULL = use command line)
        command.data(),     // Command line (mutable!)
        NULL,               // Process security attributes
        NULL,               // Thread security attributes
        FALSE,              // Inherit handles
        CREATE_NEW_CONSOLE, // Flags: new console, detached
        NULL,               // Use parent's environment
        userDir,            // Use parent's current directory
        &si,                // Startup info
        &pi                 // Process info
    );

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

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
