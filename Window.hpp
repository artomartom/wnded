
#ifndef WINDOW_CORE_HPP
#define WINDOW_CORE_HPP

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>
#endif //_WINDOWS_

#include "WindowFlags.hpp"
#include "Handle.hpp"
#include "EventHandlers.hpp"

namespace Window
{

  Handle Create(RECT rect, Style style, const Handle *parent, IProcCallback *callback);
  void ShowWindow(const Handle &handle);

  class Impl : public IProcCallback, public Handle
  {
  };
  void InitWindow(RECT rect, Impl::Handle *parent, Impl *impl);
  inline UINT PeekEvent(const Handle &handle)
  {
    static ::MSG messages{};
    ::PeekMessageW(&messages, handle.Get(), 0, 0, PM_REMOVE);
    ::TranslateMessage(&messages);
    ::DispatchMessageW(&messages);
    return messages.message;
  };
  inline UINT GetEvent(const Handle &handle)
  {
    static ::MSG messages{};
    ::GetMessageW(&messages, handle.Get(), 0, 0);
    ::TranslateMessage(&messages);
    ::DispatchMessageW(&messages);
    return messages.message;
  };
   

  //Certain window data is cached, so changes you make using SetWindowLongPtr
  // will not take effect until you call the SetWindowPos function.

};

#endif // WINDOW_CORE_HPP