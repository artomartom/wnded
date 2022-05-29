/*
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
*/

namespace Window
{
    struct CreationArgs;
    struct SizeChangedArgs;
    struct AppEventArgs;
    struct CursorArgs;
    struct CommandArgs;
    struct KeyEventArgs;
    struct ActivateArgs;

    class IProcCallback
    {
    public:
        /**
         * Event proc message handlers
         */
        virtual void OnCreate(_In_ const ::Window::CreationArgs &args) noexcept  {};
        virtual void OnPaint() noexcept  {};
        virtual void OnClose() noexcept  {};
        virtual void OnKeyHold(_In_ const ::Window::KeyEventArgs &args) noexcept  {};
        virtual void OnKeyStroke(_In_ const ::Window::KeyEventArgs &args) noexcept  {};
        virtual void OnCursorMove(_In_ const ::Window::CursorArgs &args) noexcept  {};
        virtual void OnCursorEvent(_In_ const ::Window::CursorArgs &args) noexcept  {};
        virtual void OnSizing(_Inout_updates_opt_(1) RECT *pRect) noexcept  {};
        virtual void OnCommand(_In_ const ::Window::CommandArgs &args) noexcept  {};
        virtual void OnSizeChanged(_In_ const ::Window::SizeChangedArgs &args) noexcept  {};
        virtual void OnWindowActivate(_In_ const ::Window::ActivateArgs &args) noexcept  {};
        virtual void OnAppEvent(_In_ const ::Window::AppEventArgs &args) noexcept  {};
    };

}
