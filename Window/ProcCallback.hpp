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
        virtual void OnCreate(_In_ const CreationArgs &args) noexcept = 0;
        virtual void OnPaint() noexcept = 0;
        virtual void OnClose() noexcept = 0;
        virtual void OnKeyHold(_In_ const KeyEventArgs &args) noexcept = 0;
        virtual void OnKeyStroke(_In_ const KeyEventArgs &args) noexcept = 0;
        virtual void OnCursorMove(_In_ const CursorArgs &args) noexcept = 0;
        virtual void OnCursorEvent(_In_ const CursorArgs &args) noexcept = 0;
        virtual void OnSizing(_Inout_updates_opt_(1) RECT *pRect) noexcept = 0;
        virtual void OnCommand(_In_ const CommandArgs &args) noexcept = 0;
        virtual void OnSizeChanged(_In_ const SizeChangedArgs &args) noexcept = 0;
        virtual void OnWindowActivate(_In_ const ActivateArgs &args) noexcept = 0;
        virtual void OnAppEvent(_In_ const AppEventArgs &args) noexcept = 0;
    };

}
