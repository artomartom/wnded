
#include "Hello//Hello.hpp"
#include "Window/Window.hpp"

// Log<Console>::Write(__LINE__);

class ParentWindow : public Window::IProcCallback, public Window::Handle
{
public:
    ParentWindow() = default;
    void OnCreate(_In_ const ::Window::CreationArgs &args) noexcept override { Log<Console>::Write(L"Create Parent"); };
    void OnCursorEvent(_In_ const ::Window::CursorArgs &args) noexcept override{/*Log<Console>::Write(L"cursor"); */};
    void OnWindowActivate(_In_ const ::Window::ActivateArgs &args) noexcept override{/* Log<Console>::Write(L"Activate"); */};
};
class Child : public ParentWindow
{
    void OnCreate(_In_ const ::Window::CreationArgs &args) noexcept override { Log<Console>::Write(L"Create Child"); };
};

int s_main()
{
#if 1

    ParentWindow pw{};
    Child cw{};
    Window::InitWindow(L"parent", RECT{100, 100, 1000, 1000}, &pw);
    Window::InitChildWindow(L"child", RECT{50, 50, 100, 200}, &pw, &cw);

    ::MSG messages{};
    while (messages.message != WM_QUIT)
    {
        ::PeekMessageW(&messages, 0, 0, 0, PM_REMOVE);
        ::TranslateMessage(&messages);
        ::DispatchMessageW(&messages);
    };

#endif

    MessageBeep(5);
    return 0;
};
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
int wWinMain(_In_ HINSTANCE hinst, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int) { return Invoke(s_main); };
int main() { return Invoke(s_main); };
