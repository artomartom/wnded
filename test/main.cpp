
#include "Hello//Hello.hpp"
#include "Window/Window.hpp"

// Log<Console>::Write(__LINE__);

#define CASE(message, action) \
    case message:             \
        action;               \
        break

using namespace Window;

const std::wstring translateCursorArgsMisc(enum CursorArgs::Misc In)
{

    switch (In)
    {

        CASE(CursorArgs::Misc::None_Misc, return L"None";);
        CASE(CursorArgs::Misc::CtrlDown_Misc, return L"CtrlDown";);
        CASE(CursorArgs::Misc::LBDown_Misc, return L"LBDown";);
        CASE(CursorArgs::Misc::MBDown_Misc, return L"MBDown";);
        CASE(CursorArgs::Misc::RBDown_Misc, return L"RBDown";);
        CASE(CursorArgs::Misc::ShiftDown_Misc, return L"ShiftDown";);
        CASE(CursorArgs::Misc::X1Down_Misc, return L"X1Down";);
        CASE(CursorArgs::Misc::X2Down_Misc, return L"X2Down";);
    default:
        return L"CursorArgsMisc:undefined";
    };
};

const std::wstring translateCursorArgsEvent(enum CursorArgs::Event In)
{
    switch (In)
    {
        CASE(CursorArgs::Event::LUp, return L"LUp";);
        CASE(CursorArgs::Event::LDown, return L"LDown";);
        CASE(CursorArgs::Event::LDoubClick, return L"LDoubClick";);
        CASE(CursorArgs::Event::MUp, return L"MUp";);
        CASE(CursorArgs::Event::MDown, return L"MDown";);
        CASE(CursorArgs::Event::MDoubClick, return L"MDoubClick";);
        CASE(CursorArgs::Event::RUp, return L"RUp";);
        CASE(CursorArgs::Event::RDown, return L"RDown";);
        CASE(CursorArgs::Event::RDoubClick, return L"RDoubClick";);
    default:
        return L"CursorArg:undefined";
    };
};
class test : public Window::Impl
{
public:
    static int AppEntry(HINSTANCE hinst)
    {
        DBG_ONLY(_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF));
        test pw{};
        testChild cw{};
        Window::InitWindow(RECT{100, 100, 500, 800}, nullptr, &pw);
        Window::InitWindow(RECT{50, 50, 100, 200}, &pw, &cw);
        while (Window::GetEvent(pw) != WM_QUIT)
        {
        };
        MessageBeep(5);
        return 0;
    };

    CreationArgs::Status OnCreate(_In_ const CreationArgs &args) noexcept override
    {
        Log<Console>::Write(L"OnCreate Parent");
        SIZE RTSize{RECTWIDTH(args.rect), RECTHEIGHT(args.rect)};
        return CreationArgs::Status{};
    };
    CloseArgs::Status OnClose(const CloseArgs &args) noexcept
    {
        Log<Console>::Write(L"OnClose Parent");
        return CloseArgs::Status{};
    };

    ActivateArgs::Status OnWindowActivate(_In_ const ActivateArgs &args) noexcept
    {
        return ActivateArgs::Status{};
        Log<Console>::Write(L"OnWindowActivate");
    };

    KeyEventArgs::Status OnKeyStroke(_In_ const KeyEventArgs &args) noexcept
    {
        return KeyEventArgs::Status{};
        switch (args.virtualKey)
        {
            CASE(VK_ESCAPE, {});
            CASE(VK_SPACE, {});
        }
        Log<Console>::Write(L"OnKeyStroke");
    };

    SizeChangedArgs::Status OnSizeChanged(_In_ const SizeChangedArgs &args) noexcept
    {
        Log<Console>::Write(L"OnSizeChanged");
        return SizeChangedArgs::Status{};
    };
    CursorArgs::Status OnCursorMove(_In_ const CursorArgs &args) noexcept

    {
        return CursorArgs::Status{};

        Log<Console>::Write(L"OnCursorMove", L"val:", static_cast<UINT>(args.event),
                            L"Misc:", translateCursorArgsMisc(args.misc), L"Misc val:", static_cast<WPARAM>(args.misc));
    };
    CursorArgs::Status OnCursorEvent(_In_ const CursorArgs &args) noexcept
    {
        return CursorArgs::Status{};
        Log<Console>::Write(L"OnCursorEvent Event:", translateCursorArgsEvent(args.event), L"Event val:", static_cast<UINT>(args.event),
                            L"cursor pos:", args.pos.x, args.pos.y,
                            L"Misc:", translateCursorArgsMisc(args.misc), L"Misc val:", static_cast<WPARAM>(args.misc));
    };

    void Draw() noexcept
    {
        Log<Console>::Write(L"Draw");
    };

    void OnClose() noexcept
    {
        Log<Console>::Write(L"OnClose");
    };
};
class testChild : public test
{
    CreationArgs::Status OnCreate(_In_ const CreationArgs &args) noexcept override
    {
        Log<Console>::Write(L"Create Child");
        return CreationArgs::Status{};
    };
    CloseArgs::Status OnClose(const CloseArgs &args) noexcept
    {
        Log<Console>::Write(L"OnClose Child");
        return CloseArgs::Status{};
    };
};

int s_main()
{
#if 1

#endif
};
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
int wWinMain(_In_ HINSTANCE hinst, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int) { return Invoke(test::AppEntry); };
int main() { return Invoke(s_main); };
