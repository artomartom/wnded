
#include "WindowCore.hpp"
#include "EventHandlers.hpp"

#include "../Hello//Hello.hpp"
namespace Window
{
  static ::LRESULT __stdcall CoreProcedure(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam);
  LPCWSTR RegisterWindowClass(LPCWSTR className)
  {
    WNDCLASSEXW wincl{
        sizeof(WNDCLASSEXW),
        0,
        CoreProcedure,
        0, 0, GetModuleHandleW(NULL), 0, ::LoadCursorA(0, IDC_ARROW), 0, 0,
        className, 0};

    if (::RegisterClassExW(&wincl))
    {
      return className;
    }
    else
    {
      return nullptr;
    };
  };
  WindowCore::WindowCore(IProcCallback *callback, LPCWSTR className, RECT rect)
      : m_rect{rect},
        m_className{RegisterWindowClass(className)},
        m_handle{WindowCore::Create(callback)}
  {
    if (m_handle != 0)
    {

      ::ShowWindow(m_handle, SW_SHOWDEFAULT);
      ::UpdateWindow(m_handle);
    }
    else
    { /*error*/
    };
  };
  WindowCore::WindowCore(IProcCallback *callback, LPCWSTR className, RECT rect, const WindowCore &parent)
      : WindowCore(callback, className, rect)
  {
    if (parent.Handle() != 0 && m_handle != 0)
      ::SetParent(m_handle, parent.Handle());
  };

  HWND WindowCore::Create(IProcCallback *callback)
  {
    if (m_className != nullptr)
    {
      RECT MainRect{m_rect}; // adjusted rect

      ::AdjustWindowRectEx(&MainRect, m_style, 0, m_styleEx);
      return ::CreateWindowExW(
          m_styleEx,
          m_className, 0,
          m_style | WS_MINIMIZE, // WS_MINIMIZE - wnd always minimize on creation
          MainRect.left, MainRect.top,
          RECTWIDTH(MainRect),
          RECTHEIGHT(MainRect),
          HWND_DESKTOP, nullptr, nullptr, callback);
    }
    else
    {
      return 0;
    }
  };

  WindowCore::~WindowCore()
  {
    UnregisterClassW(m_className, 0);
    ::DestroyWindow(m_handle);
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
                 Log<Console>::Write(__LINE__);
                 ::SetWindowLongPtrW(hWnd, -21, (LONG_PTR)pCallbackable); // GWL_USERDATA
                 Log<Console>::Write(__LINE__);
                 pCallbackable->OnCreate(*createStruct);
                 Log<Console>::Write(__LINE__);
               });
      PROCCASE(WM_ACTIVATE, false, { pCallbackable->OnWindowActivate({wParam}); });
      PROCCASE(WM_CLOSE, false, { ::PostQuitMessage(0); });
      /*visual*/
      PROCCASE(WM_ERASEBKGND, true, {});
      PROCCASE(WM_PAINT, false,
               {
                 ::ValidateRect(hWnd, 0);
                 pCallbackable->OnPaint();
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
