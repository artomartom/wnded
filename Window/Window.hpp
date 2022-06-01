
#ifndef WINDOW_CORE_HPP
#define WINDOW_CORE_HPP

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>
#endif //_WINDOWS_

#include "VisualStyle.hpp"
#include "Handle.hpp"

#include "ProcCallback.hpp"

namespace Window
{
  Handle Create(IProcCallback *callback, LPCWSTR className, RECT rect, Style style, const Handle &parent);
  void ShowWindow(const Handle &handle);

  template <typename ProcCallImpl>
  void InitWindow(LPCWSTR className, RECT rect, ProcCallImpl *impl)
  {
    auto &thisHandle{static_cast<Handle &>(*impl)};
    thisHandle = Create(impl,
                        className,
                        rect,
                        Style{},
                        Handle{});
    if (thisHandle.IsValid())
    {
      ShowWindow(thisHandle);
    }
    else
    { /*error*/
    Error<::Console>::Write(__LINE__);
    };
  };

  template <typename ProcCallImpl,typename ParentProcCallImpl>
  void InitChildWindow(LPCWSTR className, RECT rect, ParentProcCallImpl *parent, ProcCallImpl *impl)
  {
    auto &thisHandle{static_cast<Handle &>(*impl)};
    thisHandle = Create(impl,
                        className,
                        rect,
                        Style{},
                        static_cast<Handle &>(*parent));
    if (thisHandle.IsValid())
    {
      ShowWindow(thisHandle);
    }
    else
    { /*error*/
    };
  };
};

#endif // WINDOW_CORE_HPP