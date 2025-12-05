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

inline std::map<GUID, HWND, GUIDComparer> focusCache;

inline void cacheFocus(const GUID& desktop, HWND window)
{
    if (IsWindow(window))
        focusCache[desktop] = window;
    else
        focusCache.erase(desktop);
}

inline HWND decacheFocus(const GUID& desktop)
{
    auto iterator = focusCache.find(desktop);
    if (iterator == focusCache.end())
        return nullptr;

    HWND window = iterator->second;
    focusCache.erase(iterator);
    
    return IsWindow(window) ? window : nullptr;
}

inline void setFocus(HWND window)
{
    if (IsWindow(window))
    {
        ShowWindow(window, IsIconic(window) ? SW_RESTORE : SW_SHOW);
        SetForegroundWindow(window);
    }
}

#endif // FOCUS_HPP
