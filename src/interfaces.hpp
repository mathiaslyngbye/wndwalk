#pragma once

#include <Unknwn.h>    // IUnknown
#include <initguid.h>  // DEFINE_GUID

// CLSIDs
DEFINE_GUID(CLSID_ImmersiveShell,                   0xC2F03A33, 0x21F5, 0x47FA, 0xB4, 0xBB, 0x15, 0x63, 0x62, 0xA2, 0xF2, 0x39);
DEFINE_GUID(CLSID_VirtualDesktopManagerInternal,    0xC5E0CDCA, 0x7B6E, 0x41B2, 0x9F, 0xC4, 0xD9, 0x39, 0x75, 0xCC, 0x46, 0x7B);

// IIDs
DEFINE_GUID(IID_IVirtualDesktopManagerInternal,     0x53F5CA0B, 0x158F, 0x4124, 0x90, 0x0C, 0x05, 0x71, 0x58, 0x06, 0x0B, 0x27);

// Internal undocumented COM interface
struct IVirtualDesktopManagerInternal : public IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE GetCount(UINT* count) = 0;
    virtual HRESULT STDMETHODCALLTYPE MoveViewToDesktop(IUnknown* view, IUnknown* desktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE CanViewMoveDesktops(IUnknown* view, INT* canMove) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCurrentDesktop(IUnknown** desktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDesktops(IUnknown** desktops) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAdjacentDesktop(IUnknown* from, UINT direction, IUnknown** to) = 0;
    virtual HRESULT STDMETHODCALLTYPE SwitchDesktop(IUnknown* desktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateDesktop(IUnknown** desktop) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveDesktop(IUnknown* desktop, IUnknown* fallback) = 0;
    virtual HRESULT STDMETHODCALLTYPE FindDesktop(GUID* id, IUnknown** desktop) = 0;
};
