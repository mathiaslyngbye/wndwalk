#ifndef FOCUS_HPP
#define FOCUS_HPP

#include <unordered_map>
#include <Windows.h>
#include <rpc.h>

struct GUIDHasher
{
    std::size_t operator()(const GUID& guid) const noexcept
    {
        RPC_STATUS status;
        return UuidHash(const_cast<GUID*>(&guid), &status);
    }
};

inline std::unordered_map<GUID, HWND, GUIDHasher> focusCache;

inline void cacheFocus(const GUID& desktop, HWND window)
{
    if (IsWindow(window))
    {
        focusCache[desktop] = window;
    }
}

inline HWND decacheFocus(const GUID& desktop)
{
    auto iterator = focusCache.find(desktop);
    if (iterator != focusCache.end() && IsWindow(iterator->second))
    {
        return iterator->second;
    }
    return nullptr;
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
