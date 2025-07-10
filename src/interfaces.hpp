#ifndef INTERFACES_HPP
#define INTERFACES_HPP

#include <Unknwn.h>    // IUnknown
#include <initguid.h>  // DEFINE_GUID

// Fix clashing function definitions
#undef CreateDesktop

// CLSIDs
DEFINE_GUID(CLSID_ImmersiveShell,                   0xC2F03A33, 0x21F5, 0x47FA, 0xB4, 0xBB, 0x15, 0x63, 0x62, 0xA2, 0xF2, 0x39);
DEFINE_GUID(CLSID_VirtualDesktopManagerInternal,    0xC5E0CDCA, 0x7B6E, 0x41B2, 0x9F, 0xC4, 0xD9, 0x39, 0x75, 0xCC, 0x46, 0x7B);

// IVirtualDesktop
struct __declspec(uuid("3F07F4BE-B107-441A-AF0F-39D82529072C"))
IVirtualDesktop : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE IsViewVisible(IUnknown* view, BOOL* visible) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetID(GUID* id) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetName(HSTRING* name) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetWallpaper(HSTRING* wallpaper) = 0;
};

// IVirtualDesktopManagerInternal
struct __declspec(uuid("53F5CA0B-158F-4124-900C-057158060B27"))
IVirtualDesktopManagerInternal : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetCount(UINT* count) = 0;
    virtual HRESULT STDMETHODCALLTYPE MoveViewToDesktop(IUnknown* view, IUnknown* desktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE CanViewMoveDesktops(IUnknown* view, INT* canMove) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCurrentDesktop(IUnknown** desktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDesktops(IUnknown** desktops) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAdjacentDesktop(IUnknown* from, UINT direction, IUnknown** to) = 0;
    virtual HRESULT STDMETHODCALLTYPE SwitchDesktop(IUnknown* desktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE SwitchDesktopAndMoveForegroundView(IUnknown* desktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateDesktop(IUnknown** desktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveDesktop(IUnknown* desktop, IUnknown* fallback) = 0;
    virtual HRESULT STDMETHODCALLTYPE FindDesktop(GUID* id, IUnknown** desktop) = 0;
};

struct __declspec(uuid("372E1D3B-38D3-42E4-A15B-8AB2B178F513"))
IApplicationView : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE SetFocus() = 0;
    virtual HRESULT STDMETHODCALLTYPE SwitchTo() = 0;
    virtual HRESULT STDMETHODCALLTYPE TryInvokeBack(IUnknown* callback) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetThumbnailWindow(HWND* hwnd) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetMonitor(HMONITOR* monitor) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetVisibility(int* visible) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetVirtualDesktopId(GUID* id) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetVirtualDesktopId(const GUID* id) = 0;
};

struct __declspec(uuid("1841C6D7-4F9D-42C0-AF41-8747538F10E5"))
IApplicationViewCollection : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetViews(IUnknown** array) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewsByZOrder(IUnknown** array) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewsByAppUserModelId(PCWSTR id, IUnknown** array) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewForHwnd(HWND hwnd, IApplicationView** view) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewForApplication(IUnknown* app, IApplicationView** view) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewForAppUserModelId(PCWSTR id, IApplicationView** view) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetViewInFocus(IApplicationView** view) = 0;
    virtual HRESULT STDMETHODCALLTYPE TryGetLastActiveVisibleView(IApplicationView** view) = 0;
    virtual HRESULT STDMETHODCALLTYPE RefreshCollection() = 0;
    virtual HRESULT STDMETHODCALLTYPE RegisterForApplicationViewChanges(IUnknown* listener, DWORD* id) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnregisterForApplicationViewChanges(DWORD id) = 0;
};

#endif // INTERFACES_HPP