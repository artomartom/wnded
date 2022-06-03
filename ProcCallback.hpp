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

 

}
