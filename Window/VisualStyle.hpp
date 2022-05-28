
#ifndef WINDOW_VISUAL_HPP
#define WINDOW_VISUAL_HPP

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#define RECTWIDTH(x) (x.right - x.left)
#define RECTHEIGHT(x) (x.bottom - x.top)

namespace Window
{
    enum Style : ::DWORD
    {
        SysMenu = WS_SYSMENU,
        Minimizable = WS_MINIMIZEBOX,
        Maximizable = WS_MAXIMIZEBOX,
        Resizable = WS_THICKFRAME,
        Borber = WS_BORDER,
        TitleBar = WS_CAPTION,
    };
}
#endif // WINDOW_VISUAL_HPP
