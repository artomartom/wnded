
#ifndef WINDOW_VISUAL_HPP
#define WINDOW_VISUAL_HPP

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#define RECTWIDTH(x) (x.right - x.left)
#define RECTHEIGHT(x) (x.bottom - x.top)

namespace Window
{
    struct Style
    {
        // Style(enum Regular r, enum Extended ex)
        //     : regular{r}, extended{ex} {};

        Style()
            : regular{Regular::SysMenu |
                      Regular::Minimizable |
                      Regular::Maximizable |
                      Regular::Resizable |
                      Regular::Borber |
                      Regular::TitleBar},
              extended{Extended::OverlappedWindow} {};
        enum Regular : ::DWORD
        {
            SysMenu = WS_SYSMENU,
            Minimizable = WS_MINIMIZEBOX,
            Maximizable = WS_MAXIMIZEBOX,
            Resizable = WS_THICKFRAME,
            Borber = WS_BORDER,
            Minimized = WS_MINIMIZE,
            Child = WS_CHILD,
            TitleBar = WS_CAPTION,
        } regular{};
        enum Extended : ::DWORD
        {
            OverlappedWindow = WS_EX_OVERLAPPEDWINDOW,

        } extended{};

        auto GetRegular() const noexcept { return regular; };
        auto GetExtended() const noexcept { return extended; };
    };
}
#endif // WINDOW_VISUAL_HPP
