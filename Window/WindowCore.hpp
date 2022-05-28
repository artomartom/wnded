
#ifndef WINDOW_CORE_HPP
#define WINDOW_CORE_HPP

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif //_WINDOWS_

#include "VisualStyle.hpp"

#include "ProcCallback.hpp"

namespace Window
{

  

  class WindowCore
  {
  public:
    WindowCore(IProcCallback *callback, LPCWSTR className, RECT rect);
    WindowCore(IProcCallback *callback, LPCWSTR className, RECT rect, const WindowCore &parent);
    virtual ~WindowCore();

    HWND Create(IProcCallback *callback);

    bool IsValid() noexcept { return m_handle != 0; };
    HWND Handle() const noexcept { return m_handle; };

    // implementation calls this function to programmatically close itself
    //    void Close() noexcept { ::SendMessageW(m_handle, WM_CLOSE, 0, 0); };
  private:
    RECT m_rect{};
    LPCWSTR m_className{};
    DWORD m_styleEx{WS_EX_OVERLAPPEDWINDOW};
    Style m_style{
        Style::SysMenu |
        Style::Minimizable |
        Style::Maximizable |
        Style::Resizable |
        Style::Borber |
        Style::TitleBar};
    HWND m_handle{};
  };

  template <typename ProcCallImpl>
  class WindowCoreImpl :  public WindowCore,public ProcCallImpl 
  {
  public:
    WindowCoreImpl(LPCWSTR className, RECT rect)
        : WindowCore(static_cast<IProcCallback*>(this), className, rect){};
    WindowCoreImpl(LPCWSTR className, RECT rect, const WindowCore &parent)
        : WindowCore(static_cast<IProcCallback*>(this), className, rect){};
  };

}

#endif // WINDOW_CORE_HPP