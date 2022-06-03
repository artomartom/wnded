#pragma once
namespace Window
{
    class Handle
    {

    public:
        Handle() = default;

        explicit Handle(HWND handle) : m_handle{handle} {};
        // explicit Handle(nullptr_t) : m_handle{nullptr} {};

        Handle(const Handle &) = delete;
        Handle &operator=(const Handle &) = delete;

        Handle(Handle &&other)
            : m_handle{other.m_handle}
        {
            other.m_handle = 0;
        };
        Handle &operator=(Handle &&other)
        {
            swap(*this, other);
            return (*this);
        };
        static void swap(Handle &left, Handle &right)
        {
            HWND tmp{left.m_handle};
            left.m_handle = right.m_handle;
            right.m_handle = tmp;
        };
        virtual ~Handle()
        {
            if (IsValid())
            {
                ::DestroyWindow(m_handle);
            };
        };
        bool IsValid() const noexcept { return m_handle != nullptr; }
        HWND Get() const noexcept { return m_handle; };

    private:
        HWND m_handle{nullptr};
    };
    
};