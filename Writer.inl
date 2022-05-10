
#ifndef WRITER_INL
#define WRITER_INL

namespace Writer
{

    inline void cOut(std::wstring_view text)
    {

        std::wcout << text << L'\n';
    };

    template <typename... Args>
    constexpr std::wstringstream Accumulate(Args const &...args)
    {
        std::wstringstream ss{};
        (void)std::initializer_list<int>{(ss << args << L' ', 0)...};
        return ss;
    };

    inline std::wstring GetSysTime()
    {
        auto time{Time::GetLocalTime()};

        std::wstring out{L"00:00:00|00.00| "};
        WORD t[]{
            time.wHour, time.wMinute, time.wSecond, time.wDay, time.wMonth};
        UINT i{};
        for (auto &each : t)
        {
            out[i] += each / 10u;
            out[i + 1] += each % 10u;
            i += 3;
        };
        return out;
    };

    template <Type T>
    constexpr std::wstring_view prefix()
    {
#define CASE(in, out) \
    case in:          \
        return out
        switch (T)
        {
            CASE(Type::None, L"None     |");
            CASE(Type::Log, L"Log      |");
            CASE(Type::Warning, L"Warning  |");
            CASE(Type::Error, L"Error    |");
        default:
            return L"          ";
        }
#undef CASE
    };

    template <Type MT, Out O>
    struct Message
    {
    };

    template <Type MT>
    struct Message<MT, Out::Console>
    {
        template <typename... Args>
        constexpr static void Write(Args const &...args)
        {
            cOut(Accumulate(prefix<MT>(), GetSysTime(), args...).view());
        };
    };
    template <Type MT>
    struct Message<MT, Out::File>
    {
        template <typename... Args>
        constexpr static void Write(Args const &...args)
        {
            Wrapper::HFile file{Wrapper::HFile::CreateFile(L"Log.txt")};
            file.Write(Accumulate(prefix<MT>(), GetSysTime(), args...).view());
            file.Write(L"\n");
        };
    };

};
#endif