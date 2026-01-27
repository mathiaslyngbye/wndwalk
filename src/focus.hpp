#ifndef FOCUS_HPP
#define FOCUS_HPP

#include <map>
#include <Windows.h>

struct GUIDComparer
{
    bool operator()(const GUID& a, const GUID& b) const noexcept
    {
        return memcmp(&a, &b, sizeof(GUID)) < 0;
    }
};

inline std::map<GUID, HWND, GUIDComparer> focus_cache;

inline void cache_focus(const GUID& desktop, HWND window)
{
    if (IsWindow(window))
        focus_cache[desktop] = window;
    else
        focus_cache.erase(desktop);
}

inline HWND decache_focus(const GUID& desktop)
{
    auto iterator = focus_cache.find(desktop);
    if (iterator == focus_cache.end())
        return nullptr;

    HWND window = iterator->second;
    focus_cache.erase(iterator);

    return IsWindow(window) ? window : nullptr;
}

inline void set_focus(HWND window)
{
    if (!IsWindow(window))
        return;

    int command = SW_SHOW;
    if(IsIconic(window))
        command = SW_RESTORE;
    else if(IsZoomed(window))
        command = SW_SHOWMAXIMIZED;

    ShowWindow(window, command);
    SetForegroundWindow(window);
}

#endif
