
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
    WindowCore() = default;
    WindowCore(
        IProcCallback *callback,
        LPCWSTR className,
        RECT rect,
        Style style,
        const WindowCore &parent

    );
    WindowCore(const WindowCore &) = delete;
    WindowCore(WindowCore &&) = delete;
    virtual ~WindowCore();

    static HWND Create(IProcCallback *callback, LPCWSTR className, RECT rect, Style style);

    bool IsValid() noexcept { return m_handle != 0; };
    HWND Handle() const noexcept { return m_handle; };

    // implementation calls this function to programmatically close itself
    //    void Close() noexcept { ::SendMessageW(m_handle, WM_CLOSE, 0, 0); };

  private:
    HWND m_handle{};
  };

  template <typename ProcCallImpl>
  struct WindowCoreImpl : public ProcCallImpl, public WindowCore
  {

    WindowCoreImpl(LPCWSTR className, RECT rect)
        : WindowCore(this,
                     className,
                     rect,
                     Style{Style::Regular::SysMenu |
                               Style::Regular::Minimizable |
                               Style::Regular::Maximizable |
                               Style::Regular::Resizable |
                               Style::Regular::Borber |
                               Style::Regular::TitleBar,
                           Style::Extended::OverlappedWindow},
                     WindowCore{}){};
  };

  template <typename ProcCallImpl>
  struct WindowChildImpl : public ProcCallImpl, public WindowCore
  {
    WindowChildImpl(LPCWSTR className, RECT rect, const WindowCore &parent)
        : WindowCore(this, className, rect,
                     Style(Style::Regular::SysMenu |
                               Style::Regular::Minimizable |
                               Style::Regular::Maximizable |
                               Style::Regular::Resizable |
                               Style::Regular::Borber |
                               Style::Regular::TitleBar,
                           Style::Extended::OverlappedWindow)
                           ,
                     parent){};
  };
};

#endif // WINDOW_CORE_HPP