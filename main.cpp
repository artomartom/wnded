
#include "Window/WindowCore.hpp"

#include "Hello//Hello.hpp"

using namespace Window;

// Log<Console>::Write(__LINE__);
class ParentProcCallback : public IProcCallback
{
    void OnCreate(_In_ const ::Window::CreationArgs &args) noexcept override { Log<Console>::Write(L"Create"); };
    void OnCursorEvent(_In_ const ::Window::CursorArgs &args) noexcept override { Log<Console>::Write(L"cursor"); };
};
class ParentWindow : public WindowCoreImpl<ParentProcCallback>
{
public:
    ParentWindow()
        : WindowCoreImpl{L"parent", RECT{100, 100, 1000, 1000}} {};
};

class child1Window : public WindowChildImpl<ParentProcCallback>
{
public:
    child1Window(const WindowCore &parent)
        : WindowChildImpl{L"child1", RECT{300, 200, 400, 400}, parent} {};
};
class child2Window : public WindowChildImpl<ParentProcCallback>
{
public:
    child2Window(const WindowCore &parent)
        : WindowChildImpl{L"child2", RECT{50, 50, 300, 200}, parent} {};
};

int s_main()
{
#if 1

    ParentWindow pw{};
    child1Window cw1{pw};
    child2Window cw2{pw};

    if (!(pw.IsValid() && cw1.IsValid() &&
          cw2.IsValid()))
        return -1;

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
