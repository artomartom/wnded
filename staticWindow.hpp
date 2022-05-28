
#ifndef WINDOW_HPP
#define WINDOW_HPP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "EventHandlers.hpp"

namespace Window
{

  template <class TImpl>
  class WindowCore : public TImpl
  {

  public:
    WindowCore() = delete;
    WindowCore(const WindowCore &) = delete;
    WindowCore(WindowCore &&) = delete;
    WindowCore &operator=(const WindowCore &) = delete;
    WindowCore &operator=(WindowCore &&) = delete;
    ~WindowCore()
    {
      TImpl::OnClose();
      ::DestroyWindow(TImpl::m_handle);
      TImpl::m_handle = nullptr;
    };
    struct packedCreateArgs_t
    {
      WindowCore<TImpl> *pthis{};
      CreationArgs args{};
    };
    WindowCore(HINSTANCE hinst, const RECT rect)
    {

      WNDCLASSEXW wincl{
          sizeof(WNDCLASSEXW),
          CS_GLOBALCLASS | CS_PARENTDC,
          CoreProcedure,
          0, 0, GetModuleHandleW(NULL), 0, ::LoadCursorA(0, IDC_ARROW), 0, 0,
          L"WindowCore", 0};
      ::RegisterClassExW(&wincl);

      RECT MainRect{rect};
      ::AdjustWindowRectEx(&MainRect, TImpl::m_style, 0, TImpl::m_styleEx);

      packedCreateArgs_t createArgsTpl{this, CreationArgs{hinst, rect}};

      ::CreateWindowExW(
          TImpl::m_styleEx,
          wincl.lpszClassName, 0,
          TImpl::m_style,
          MainRect.left, MainRect.top,
          RECTWIDTH(MainRect),
          RECTHEIGHT(MainRect),
          HWND_DESKTOP, nullptr, nullptr, &createArgsTpl);
    };

  private:
    template <class WindowCoreSpec>
    friend int __stdcall Run(const WindowCoreSpec &window);

    inline static ::LRESULT __stdcall CoreProcedure(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
    {

#define PROCCASE(message, boolreturn, action) \
  case message:                               \
    action;                                   \
    return boolreturn

      WindowCore *pthis{(WindowCore *)::GetWindowLongPtrW(hWnd, -21)}; // GWL_USERDATA
      switch (message)
      {
        /*window status */
        PROCCASE(WM_CREATE,
                 false,
                 {
                   CREATESTRUCT *createStruct{(reinterpret_cast<::CREATESTRUCT *>(lParam))};
                   auto *tupl{reinterpret_cast<packedCreateArgs_t *>(createStruct->lpCreateParams)};
                   pthis = tupl->pthis; // needed to call OnCreate
                   pthis->TImpl::m_handle = hWnd;
                   ::SetWindowLongPtrW(pthis->TImpl::m_handle, -21, (LONG_PTR)pthis); // GWL_USERDATA
                   pthis->TImpl::OnCreate(tupl->args);
                     ::ShowWindow(hWnd, SW_SHOWDEFAULT);
                     ::UpdateWindow(hWnd);
                 });
        PROCCASE(WM_ACTIVATE, false, { pthis->TImpl::OnWindowActivate({wParam}); });
        PROCCASE(WM_CLOSE, false, { ::PostQuitMessage(0); });
        /*visual*/
        PROCCASE(WM_ERASEBKGND, true, {});
        PROCCASE(WM_PAINT, false,
                 {
                   ::ValidateRect(hWnd, 0);
                   pthis->TImpl::OnPaint();
                 };);
        /*window size*/
        PROCCASE(WM_SIZE, false, pthis->TImpl::OnSizeChanged({wParam, lParam}));
        PROCCASE(WM_SIZING, true, pthis->TImpl::OnSizing(reinterpret_cast<RECT *>(lParam)));
        PROCCASE(WM_ENTERSIZEMOVE, false, { ::Sleep(30); });
        PROCCASE(WM_EXITSIZEMOVE, false, { ::Sleep(30); });

#pragma region user_input
        PROCCASE(WM_MOUSEMOVE, false, pthis->OnCursorMove(CursorArgs{CursorArgs::Event::Move, wParam, lParam}));
        // left button
        PROCCASE(WM_LBUTTONUP, false, pthis->OnCursorEvent(CursorArgs(CursorArgs::Event::LUp, wParam, lParam)));
        PROCCASE(WM_LBUTTONDOWN, false, pthis->OnCursorEvent(CursorArgs(CursorArgs::Event::LDown, wParam, lParam)));
        PROCCASE(WM_LBUTTONDBLCLK, false, pthis->OnCursorEvent(CursorArgs(CursorArgs::Event::LDoubClick, wParam, lParam)));
        // middle button
        PROCCASE(WM_MBUTTONUP, false, pthis->OnCursorEvent(CursorArgs(CursorArgs::Event::MUp, wParam, lParam)));
        PROCCASE(WM_MBUTTONDOWN, false, pthis->OnCursorEvent(CursorArgs(CursorArgs::Event::MDown, wParam, lParam)));
        PROCCASE(WM_MBUTTONDBLCLK, false, pthis->OnCursorEvent(CursorArgs(CursorArgs::Event::MDoubClick, wParam, lParam)));
        // right button
        PROCCASE(WM_RBUTTONUP, false, pthis->OnCursorEvent(CursorArgs(CursorArgs::Event::RUp, wParam, lParam)));
        PROCCASE(WM_RBUTTONDOWN, false, pthis->OnCursorEvent(CursorArgs(CursorArgs::Event::RDown, wParam, lParam)));
        PROCCASE(WM_RBUTTONDBLCLK, false, pthis->OnCursorEvent(CursorArgs(CursorArgs::Event::RDoubClick, wParam, lParam)));
        // keyboard
        PROCCASE(WM_KEYDOWN, false, { (!(lParam & (1LL << 31))) ? pthis->OnKeyStroke({wParam}) : pthis->OnKeyHold({wParam}); });
        // misc
        PROCCASE(WM_COMMAND, false, { pthis->TImpl::OnCommand(CommandArgs(wParam, lParam)); });
#pragma region end // user_input
      };

      if (message > WM_APP)
      {
        pthis->TImpl::OnAppEvent(AppEventArgs(message, wParam, lParam));
        return 0;
      }
      else
      {
        return ::DefWindowProcW(hWnd, message, wParam, lParam);
      };

#undef PROCCASE
    };
  };

  template <class WindowCoreSpec>
  int __stdcall RunDeferred(WindowCoreSpec &&window)
  {
    ::MSG messages{};

    while (::GetMessageW(&messages, 0, 0, 0))
    {
      ::TranslateMessage(&messages);
      ::DispatchMessageW(&messages);
    };
    return 0;
  };

  template <class WindowCoreSpec>
  int __stdcall RunImmediate(WindowCoreSpec &&window)
  {
    ::MSG messages{};
    while (messages.message != WM_QUIT)
    {
      ::PeekMessageW(&messages, 0, 0, 0, PM_REMOVE);
      ::TranslateMessage(&messages);
      ::DispatchMessageW(&messages);
    };
    return 0;
  };
};

class CoreApp
{
public:
  /**
   * Event proc message handlers
   */

 void   OnCreate(_In_ const ::Window::CreationArgs &args) noexcept { UNREFERENCED_PARAMETER(args); };
  void   OnPaint() noexcept {/*UNREFERENCED_PARAMETER(args);*/};
  void   OnClose() noexcept {/*UNREFERENCED_PARAMETER(args);*/};
  void   OnKeyHold(_In_ const ::Window::KeyEventArgs &args) noexcept { UNREFERENCED_PARAMETER(args); };
  void   OnKeyStroke(_In_ const ::Window::KeyEventArgs &args) noexcept { UNREFERENCED_PARAMETER(args); };
  void   OnCursorMove(_In_ const ::Window::CursorArgs &args) noexcept { UNREFERENCED_PARAMETER(args); };
  void   OnCursorEvent(_In_ const ::Window::CursorArgs &args) noexcept { UNREFERENCED_PARAMETER(args); };
  void   OnSizing(_Inout_updates_opt_(1) RECT *pRect) noexcept { UNREFERENCED_PARAMETER(pRect); };
  void   OnCommand(_In_ const ::Window::CommandArgs &args) noexcept { UNREFERENCED_PARAMETER(args); };
  void   OnSizeChanged(_In_ const ::Window::SizeChangedArgs &args) noexcept { UNREFERENCED_PARAMETER(args); };
  void   OnWindowActivate(_In_ const ::Window::ActivateArgs &args) noexcept { UNREFERENCED_PARAMETER(args); };
  void   OnAppEvent(_In_ const ::Window::AppEventArgs &args) noexcept { UNREFERENCED_PARAMETER(args); };

  // implementation calls this function to programmatically close itself
  void Close() noexcept { ::SendMessageW(m_handle, WM_CLOSE, 0, 0); };

  bool IsValid() noexcept { return m_handle != nullptr; };

protected:
  HWND m_handle{nullptr};
  bool m_isVisible{};
  DWORD m_styleEx{WS_EX_OVERLAPPEDWINDOW};
  DWORD m_style{WS_OVERLAPPED |
                WS_CAPTION |
                WS_SYSMENU |
                WS_MINIMIZEBOX |
                WS_THICKFRAME |
                WS_MAXIMIZEBOX |
                WS_CAPTION |
                WS_MINIMIZE};
};

#endif
