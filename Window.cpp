
#include "Window.hpp"

namespace Window
{
  static ::LRESULT __stdcall CoreProcedure(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam);
  LPCWSTR RegisterWindowClass(LPCWSTR className, WNDPROC proc)
  {

    WNDCLASSEXW wincl{
        sizeof(WNDCLASSEXW),
        0,
        proc,
        0, 0, GetModuleHandleW(NULL), 0, ::LoadCursorA(0, (LPSTR)IDC_ARROW), 0, 0,
        className, 0};
    ::SetLastError(0);
    if (::RegisterClassExW(&wincl))
    {
      return className;
    }
    else
    {

      return nullptr;
    };
  };

  Handle Create(RECT rect, Style style, const Handle *parent, IProcCallback *callback)
  {
    static USHORT classCount{0};
    WCHAR className[15]{};
    swprintf(className, _countof(className), L"className%o", classCount);
    if (RegisterWindowClass(className, CoreProcedure) == nullptr)
    {
      return Handle{nullptr};
    }

    ++classCount;
    RECT adjustedRect{rect};
    ::AdjustWindowRectEx(&adjustedRect, style.GetRegular(), 0, style.GetExtended());
    bool HasParent{parent != nullptr};
    HWND parentHWND{HWND_DESKTOP};
    if (HasParent)
    {
      if (!parent->IsValid())
      {
        HasParent = false;
      }
      else
      {
        parentHWND = parent->Get();
        style |= Style::Regular::Child;
      };
    };
    ::SetLastError(0);
    return Handle{::CreateWindowExW(
        style.GetExtended(),
        className, 0,
        style.GetRegular() | Style::Regular::Minimized, // Minimized - wnd always minimize on creation
        adjustedRect.left, adjustedRect.top,
        RECTWIDTH(adjustedRect),
        RECTHEIGHT(adjustedRect),
        parentHWND,
        nullptr, nullptr, callback)};
  };
  void ShowWindow(const Handle &handle)
  {
    ::ShowWindow(handle.Get(), SW_SHOWDEFAULT);
    ::UpdateWindow(handle.Get());
  };

  void InitWindow(RECT rect, Impl::Handle *parent, Impl *impl)
  {
    auto &thisHandle{static_cast<Impl::Handle &>(*impl)};
    thisHandle = Create(
        rect,
        Style{},
        parent,
        static_cast<Impl::IProcCallback *>(impl));
    if (thisHandle.IsValid())
    {
      ShowWindow(thisHandle);
    }
    else
    { /*error*/
      // Error<::Console>::Write(__LINE__);
    };
  };

  static ::LRESULT __stdcall CoreProcedure(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
  {
#define PROCCASE(message, boolreturn, action) \
  case message:                               \
    action;                                   \
    return boolreturn

    IProcCallback *pCallbackable{(IProcCallback *)::GetWindowLongPtrW(hWnd, -21)}; // GWL_USERDATA

#if 1
    switch (message)
    {
      /*window status */
      PROCCASE(WM_CREATE, false,
               {
                 CREATESTRUCTW *createStruct{reinterpret_cast<::CREATESTRUCTW *>(lParam)};
                 pCallbackable = reinterpret_cast<IProcCallback *>(createStruct->lpCreateParams); // needed to call OnCreate
                 ::SetWindowLongPtrW(hWnd, -21, (LONG_PTR)pCallbackable);                         // GWL_USERDATA
                 pCallbackable->OnCreate(*createStruct);
               });
      PROCCASE(WM_ACTIVATE, false, { pCallbackable->OnWindowActivate({wParam}); });
      PROCCASE(WM_CLOSE, false, {  pCallbackable->OnClose(CloseArgs {}); ::PostQuitMessage(0); });
      /*visual*/
      PROCCASE(WM_ERASEBKGND, true, {});
      PROCCASE(WM_PAINT, false,
               {
                 ::ValidateRect(hWnd, 0);
                 pCallbackable->OnPaint(PaintArgs{});
               };);
      /*window size*/
      PROCCASE(WM_SIZE, false, pCallbackable->OnSizeChanged({wParam, lParam}));
      PROCCASE(WM_SIZING, true, pCallbackable->OnSizing(reinterpret_cast<RECT *>(lParam)));
      PROCCASE(WM_ENTERSIZEMOVE, false, { ::Sleep(30); });
      PROCCASE(WM_EXITSIZEMOVE, false, { ::Sleep(30); });

#pragma region user_input
      PROCCASE(WM_MOUSEMOVE, false, pCallbackable->OnCursorMove(CursorArgs{CursorArgs::Event::Move, wParam, lParam}));
      // left button
      PROCCASE(WM_LBUTTONUP, false, pCallbackable->OnCursorEvent(CursorArgs(CursorArgs::Event::LUp, wParam, lParam)));
      PROCCASE(WM_LBUTTONDOWN, false, pCallbackable->OnCursorEvent(CursorArgs(CursorArgs::Event::LDown, wParam, lParam)));
      PROCCASE(WM_LBUTTONDBLCLK, false, pCallbackable->OnCursorEvent(CursorArgs(CursorArgs::Event::LDoubClick, wParam, lParam)));
      // middle button
      PROCCASE(WM_MBUTTONUP, false, pCallbackable->OnCursorEvent(CursorArgs(CursorArgs::Event::MUp, wParam, lParam)));
      PROCCASE(WM_MBUTTONDOWN, false, pCallbackable->OnCursorEvent(CursorArgs(CursorArgs::Event::MDown, wParam, lParam)));
      PROCCASE(WM_MBUTTONDBLCLK, false, pCallbackable->OnCursorEvent(CursorArgs(CursorArgs::Event::MDoubClick, wParam, lParam)));
      // right button
      PROCCASE(WM_RBUTTONUP, false, pCallbackable->OnCursorEvent(CursorArgs(CursorArgs::Event::RUp, wParam, lParam)));
      PROCCASE(WM_RBUTTONDOWN, false, pCallbackable->OnCursorEvent(CursorArgs(CursorArgs::Event::RDown, wParam, lParam)));
      PROCCASE(WM_RBUTTONDBLCLK, false, pCallbackable->OnCursorEvent(CursorArgs(CursorArgs::Event::RDoubClick, wParam, lParam)));
      // keyboard
      PROCCASE(WM_KEYDOWN, false, { (!(lParam & (1LL << 31))) ? pCallbackable->OnKeyStroke({wParam}) : pCallbackable->OnKeyHold({wParam}); });
      // misc
      PROCCASE(WM_COMMAND, false, { pCallbackable->OnCommand(CommandArgs(wParam, lParam)); });
#pragma region end // user_input
    };

    if (message > WM_APP)
    {
      pCallbackable->OnAppEvent(AppEventArgs(message, wParam, lParam));
      return 0;
    }
    else
    {
      return ::DefWindowProcW(hWnd, message, wParam, lParam);
    };

#undef PROCCASE
#endif
  };
};
