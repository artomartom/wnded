
#ifndef WINDOW_EVENT_HANDLERS_HPP
#define WINDOW_EVENT_HANDLERS_HPP

namespace Window
{

    struct CreationArgs
    {
        CreationArgs(const CREATESTRUCTW &){};
        ::HINSTANCE hInst{};
        ::RECT rect{};
        using Status = int;
    };

    struct SizeChangedArgs
    {
        SizeChangedArgs(::WPARAM wParam, ::LPARAM lParam)
            : changeType{static_cast<enum ChangeType>(wParam)}, newSize{LOWORD(lParam), HIWORD(lParam)} {};
        /*args*/
        enum ChangeType : ::WPARAM
        {
            MaxHide = SIZE_MAXHIDE,     // Message is sent to all pop-up windows when some other window is maximized.
            Maximized = SIZE_MAXIMIZED, // The window has been maximized.
            MaxShow = SIZE_MAXSHOW,     // Message is sent to all pop-up windows when some other window has been restored to its former size.
            Minimized = SIZE_MINIMIZED, // The window has been minimized.
            Restored = SIZE_RESTORED,   // The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.

        } changeType{};

        struct Size
        {
            long width, height;
        } newSize{};
        using Status = int;
    };

    struct AppEventArgs
    {
        AppEventArgs(::UINT message, ::WPARAM wParam, ::LPARAM lParam)
            : wParam{wParam}, lParam{lParam}, message{message} {};
        ::WPARAM wParam{};
        ::LPARAM lParam{};
        ::ULONG message{};

        using Status = int;
    };

    struct CursorArgs
    {
        CursorArgs() = delete;
        enum Event : UINT;
        CursorArgs(Event event, ::WPARAM wParam, ::LPARAM lParam)
            : event{event},
              misc{static_cast<enum Misc>(wParam)},
              pos{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)} {};

        enum Event : UINT
        {
            None = (0u),
            Move = (1u << 0),
            // action
            Up = (1u << 2),
            Down = (1u << 3),
            DoubleClick = (1u << 4),
            // button
            Left = (1u << 5),
            Middle = (1u << 6),
            Right = (1u << 7),
            // Left
            LUp = (Left | Up),
            LDown = (Left | Down),
            LDoubClick = (Left | DoubleClick),
            // Middle
            MUp = (Middle | Up),
            MDown = (Middle | Down),
            MDoubClick = (Middle | DoubleClick),
            // Right
            RUp = (Right | Up),
            RDown = (Right | Down),
            RDoubClick = (Right | DoubleClick),

        } event{};
        enum Misc : ::WPARAM
        {
            // while main event
            None_Misc = 0,
            CtrlDown_Misc = MK_CONTROL, // The CTRL key is down.
            LBDown_Misc = MK_LBUTTON,   // The left Cursor button is down.
            MBDown_Misc = MK_MBUTTON,   // The middle Cursor button is down.
            RBDown_Misc = MK_RBUTTON,   // The right Cursor button is down.
            ShiftDown_Misc = MK_SHIFT,  // The SHIFT key is down.
            X1Down_Misc = MK_XBUTTON1,  // The first X button is down.
            X2Down_Misc = MK_XBUTTON2,  // The second X button is down.
        } misc{};
        ::POINT pos{};
        using Status = int;
    };
    struct PaintArgs
    {
        using Status = int;
    };

    struct CommandArgs
    {
        CommandArgs(::WPARAM wParam, ::LPARAM lParam)
            : wParam{wParam}, lParam{lParam} {};
        ::WPARAM wParam{};
        ::LPARAM lParam{};
        using Status = int;
    };

    struct KeyEventArgs
    {
        KeyEventArgs(::WPARAM wParam)
            : virtualKey{wParam} {};
        ::WPARAM virtualKey{}; // Key Code   ...
        using Status = int;
    };

    struct ActivateArgs
    {
        ActivateArgs(WPARAM wParam)
            : activateType{LOWORD(wParam)},
              isMinimized{static_cast<bool>(HIWORD(wParam))} {};
        /*args*/
        enum ActivateType : ::WORD
        {
            Activated = WA_ACTIVE,           // Activated by some method other than a Cursor click (  by a call to the SetActiveWindow function or by use of the keyboard interface to select the window).
            ClickActivated = WA_CLICKACTIVE, // Activated by a Cursor click.
            Deactivated = WA_INACTIVE,       // Deactivated.
        } activateType{};
        bool isMinimized{};
        using Status = int;
    };

    struct CloseArgs
    {
        using Status = int;
    };

    class IProcCallback
    {
    public:
        /**
         * Event proc message handlers
         */
        virtual CreationArgs::Status OnCreate(_In_ const CreationArgs &args) noexcept { return CreationArgs::Status{}; };
        virtual PaintArgs::Status OnPaint(const PaintArgs &args) noexcept { return PaintArgs::Status{}; };
        virtual CloseArgs::Status OnClose(const CloseArgs &args) noexcept { return CloseArgs::Status{}; };
        virtual KeyEventArgs::Status OnKeyHold(_In_ const KeyEventArgs &args) noexcept { return KeyEventArgs::Status{}; };
        virtual KeyEventArgs::Status OnKeyStroke(_In_ const KeyEventArgs &args) noexcept { return KeyEventArgs::Status{}; };
        virtual CursorArgs::Status OnCursorMove(_In_ const CursorArgs &args) noexcept { return CursorArgs::Status{}; };
        virtual CursorArgs::Status OnCursorEvent(_In_ const CursorArgs &args) noexcept { return CursorArgs::Status{}; };
        virtual void OnSizing(_Inout_updates_opt_(1) RECT *pRect) noexcept {};
        virtual CommandArgs::Status OnCommand(_In_ const CommandArgs &args) noexcept { return CommandArgs::Status{}; };
        virtual SizeChangedArgs::Status OnSizeChanged(_In_ const SizeChangedArgs &args) noexcept { return SizeChangedArgs::Status{}; };
        virtual ActivateArgs::Status OnWindowActivate(_In_ const ActivateArgs &args) noexcept { return ActivateArgs::Status{}; };
        virtual AppEventArgs::Status OnAppEvent(_In_ const AppEventArgs &args) noexcept { return AppEventArgs::Status{}; };
    };
}

#endif // WINDOW_EVENT_HANDLERS_HPP