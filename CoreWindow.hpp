
#ifndef WINDOW_HPP
#define WINDOW_HPP

#define RECTWIDTH(x) (x.right - x.left)
#define RECTHEIGHT(x) (x.bottom - x.top)

namespace Window
{

#pragma region EventArgs
  struct CreationArgs
  {

    /*args*/
    void *pCoreWindow{};
    ::HINSTANCE m_hInst{};
    ::RECT m_Rect{};
  };

  struct SizeChangedArgs
  {

    SizeChangedArgs(::WPARAM wParam, ::LPARAM lParam)
        : m_Type{static_cast<Type>(wParam)}, m_New{LOWORD(lParam), HIWORD(lParam)} {};
    /*args*/
    enum Type : ::WPARAM
    {
      MaxHide = SIZE_MAXHIDE,     // Message is sent to all pop-up windows when some other window is maximized.
      Maximized = SIZE_MAXIMIZED, // The window has been maximized.
      MaxShow = SIZE_MAXSHOW,     // Message is sent to all pop-up windows when some other window has been restored to its former size.
      Minimized = SIZE_MINIMIZED, // The window has been minimized.
      Restored = SIZE_RESTORED,   // The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.

    } m_Type{};
    ::SIZE m_New{};
  };

  struct AppEventArgs
  {
    AppEventArgs(::UINT message, ::WPARAM wParam, ::LPARAM lParam)
        :  wParam{wParam}, lParam{lParam},message{message} {};
    ::WPARAM wParam{};
    ::LPARAM lParam{};
    ::UINT message{};
  };
  struct CommandArgs
  {
    CommandArgs(::WPARAM wParam, ::LPARAM lParam)
        : wParam{wParam}, lParam{lParam} {};
    ::WPARAM wParam{};
    ::LPARAM lParam{};
  };

  struct KeyEventArgs
  {
    KeyEventArgs(::WPARAM wParam)
        : m_VirtualKey{wParam} {};
    ::WPARAM m_VirtualKey{}; // Key Code   ...
  };

  struct ActivateArgs
  {
    ActivateArgs(WPARAM wParam)
        : m_Activate{LOWORD(wParam)},
          m_IsMinimized{static_cast<bool>(HIWORD(wParam))} {};
    /*args*/
    enum Activate : ::WORD
    {
      Activated = WA_ACTIVE,           // Activated by some method other than a mouse click (  by a call to the SetActiveWindow function or by use of the keyboard interface to select the window).
      ClickActivated = WA_CLICKACTIVE, // Activated by a mouse click.
      Deactivated = WA_INACTIVE,       // Deactivated.
    } m_Activate{};
    bool m_IsMinimized{};
  };

#pragma endregion // EventArgs

  template <class TImpl>
  class CoreWindow : public TImpl
  {

  public:
    CoreWindow() = delete;
    CoreWindow(const CoreWindow &) = delete;
    CoreWindow(CoreWindow &&) = delete;
    CoreWindow &operator=(const CoreWindow &) = delete;
    CoreWindow &operator=(CoreWindow &&) = delete;
    ~CoreWindow()
    {
      TImpl::OnClose();
      ::DestroyWindow(TImpl::m_Handle);
    };

    CoreWindow(HINSTANCE hinst, const RECT rect)
    {

      WNDCLASSEXW wincl{
          sizeof(WNDCLASSEXW),
          CS_GLOBALCLASS | CS_PARENTDC,
          CoreProcedure,
          0, 0, 0, 0, ::LoadCursorA(0, IDC_ARROW), 0, 0,
          L"TheWindowName", 0};
      ::RegisterClassExW(&wincl);

      RECT MainRect{rect};
      ::AdjustWindowRectEx(&MainRect, TImpl::m_Stile, 0, TImpl::m_StileEx);

      CreationArgs CreateArgs{this, hinst, rect};
      ::CreateWindowExW(
          TImpl::m_StileEx,
          wincl.lpszClassName, 0,
          TImpl::m_Stile,
          MainRect.left, MainRect.top,
          RECTWIDTH(MainRect),
          RECTHEIGHT(MainRect),
          HWND_DESKTOP, nullptr, nullptr, &CreateArgs);
    };

  private:
    template <class CoreWindowSpec>
    friend int __stdcall Run(const CoreWindowSpec &window);

    inline static ::LRESULT __stdcall CoreProcedure(::HWND hWnd, ::UINT message, ::WPARAM wParam, ::LPARAM lParam)
    {
#define PROCCASE(message, boolreturn, action) \
  case message:                               \
    action;                                   \
    return boolreturn

      static CoreWindow *s_pthis{};

      switch (message)
      {
        /*window status */
        PROCCASE(WM_CREATE, false,
                 {
                   CreationArgs *args = reinterpret_cast<CreationArgs *>((reinterpret_cast<::CREATESTRUCT *>(lParam))->lpCreateParams);
                   s_pthis = reinterpret_cast<CoreWindow *>(args->pCoreWindow);
                   s_pthis->m_Handle = hWnd;
                   s_pthis->TImpl::OnCreate(*args);
                   ::ShowWindow(hWnd, SW_NORMAL);
                 });
        PROCCASE(WM_ACTIVATE, false, { s_pthis->TImpl::OnWindowActivate({wParam}); });
        PROCCASE(WM_CLOSE, false, { ::PostQuitMessage(0); });
        /*visual*/
        PROCCASE(WM_ERASEBKGND, true, {});
        PROCCASE(WM_PAINT, false,
                 {
                   ::ValidateRect(hWnd, 0);
                   s_pthis->TImpl::OnPaint();
                 };);
        /*window size*/
        PROCCASE(WM_SIZE, false, s_pthis->TImpl::OnSizeChanged({wParam, lParam}));
        PROCCASE(WM_SIZING, true, s_pthis->TImpl::OnSizing(reinterpret_cast<RECT *>(lParam)));
        PROCCASE(WM_ENTERSIZEMOVE, false, { ::Sleep(30); });
        PROCCASE(WM_EXITSIZEMOVE, false, { ::Sleep(30); });

        /*user input*/
        PROCCASE(WM_COMMAND, false, { s_pthis->TImpl::OnCommand(CommandArgs(wParam, lParam)); });
        PROCCASE(WM_MOUSEMOVE, false, s_pthis->OnCursorMove());
        PROCCASE(WM_KEYDOWN, false, { (!(lParam & (1LL << 31))) ? s_pthis->OnKeyStroke({wParam}) : s_pthis->OnKeyHold({wParam}); });
      };

      if (message > WM_APP)
      {
        s_pthis->TImpl::OnAppEvent(AppEventArgs(message, wParam, lParam));
        return 0;
      }
      else
      {
        return ::DefWindowProcW(hWnd, message, wParam, lParam);
      };

#undef PROCCASE
    };
  };

  template <class CoreWindowSpec>
  int __stdcall Run(const CoreWindowSpec &window)
  {
    ::MSG m_messages{};

    while (::GetMessageW(&m_messages, 0, 0, 0))
    {

      ::TranslateMessage(&m_messages);
      ::DispatchMessageW(&m_messages);
    };

    return 0;
  };
};

class CoreApp
{
public:
  /**
   * Event proc message handlers
   *
   */
  void OnCreate(_In_ const ::Window::CreationArgs &args) noexcept {};
  void OnPaint() noexcept {};
  void OnClose() noexcept {};
  void OnKeyHold(_In_ const ::Window::KeyEventArgs &args) noexcept {};
  void OnKeyStroke(_In_ const ::Window::KeyEventArgs &args) noexcept {};
  void OnCursorMove() noexcept {};
  void OnSizing(_Inout_ RECT *pRect) noexcept {};
  void OnCommand(_In_ const ::Window::CommandArgs &args) noexcept {};
  void OnSizeChanged(_In_ const ::Window::SizeChangedArgs &args) noexcept {};
  void OnWindowActivate(_In_ const ::Window::ActivateArgs &args) noexcept {};
  void OnAppEvent(_In_ const ::Window::AppEventArgs &args) noexcept {};

  void Close() noexcept { ::SendMessageW(m_Handle, WM_CLOSE, 0, 0); };

  bool IsValid() noexcept { return m_Handle != 0; };
  void SetHeader(const char *text) const noexcept { ::SetWindowTextA(m_Handle, text); };

protected:
  HWND m_Handle{nullptr};
  bool m_IsVisible{};
  DWORD m_StileEx{WS_EX_OVERLAPPEDWINDOW};
  DWORD m_Stile{WS_OVERLAPPED |
                WS_CAPTION |
                WS_SYSMENU |
                WS_MINIMIZEBOX |
                WS_THICKFRAME |
                WS_MAXIMIZEBOX |
                WS_CAPTION |
                WS_MINIMIZE};
};

#endif
