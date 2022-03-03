#ifndef POOL_ALLOC
#define POOL_ALLOC
#include <iostream>

template <typename _Tp>
class pool_alloc
{
    static_assert(!std::is_same_v<_Tp, void>, "Type of the allocator can not be void");

public:
    using allocator_type = pool_alloc<_Tp>;
    using value_type = _Tp;
    using pointer = _Tp *;
    using const_pointer = const _Tp *;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_copy_assignment = std::false_type;
    using propagate_on_container_move_assignment = std::false_type;

protected:
    static size_t max_size;        // максимальный размер памяти
    static _Tp *cur;               // указатель на первый незанятый элемент
    static size_t count_connected; // кол-во аллокаторов ссылающихся на пулл
    static _Tp *head;              // указатель на начало выделенной памяти

public:
    pool_alloc()
    {
        if (count_connected == 0)
        {
            head = reinterpret_cast<_Tp *>(::operator new(sizeof(_Tp) * max_size));
            cur = head;
        }
        ++count_connected;
    }

    // конструктор копирования
    pool_alloc(const pool_alloc<_Tp> &alloc)
    {
        head = alloc.head;
        cur = alloc.cur;
        ++count_connected;
    }

    pool_alloc(pool_alloc<_Tp> &&alloc)
    {
        head = alloc.head;
        cur = alloc.cur;
        ++count_connected;
    }

    pool_alloc<_Tp> &operator=(const pool_alloc<_Tp> &alloc)
    {
        if (this == &alloc)
            return *this;
        head = alloc.head;
        cur = alloc.cur;
        ++count_connected;
        return *this;
    }

    pool_alloc<_Tp> &operator=(pool_alloc<_Tp> &&alloc)
    {
        if (this == &alloc)
            return *this;
        head = alloc.head;
        cur = alloc.cur;
        ++count_connected;
        return *this;
    }

    _Tp *allocate(size_t n) // не заифано переполнение для скорости вычислений.
    {
        _Tp *tmp = cur;
        cur += n;
        return tmp;
    }

    void deallocate(_Tp *, size_t) {}

    size_t get_max_size() const noexcept { return max_size; }

    static size_t get_count_pointers() { return count_connected; }

    ~pool_alloc()
    {
        std::cout << "~pool_alloc\n";
        if (count_connected == 1)
            ::operator delete(head);
        else
            --count_connected;
    }

    bool operator==(const allocator_type &alloc) const { return head == alloc.head; }

    bool operator!=(const allocator_type &alloc) const { return head != alloc.head; }
};

template <typename _Tp>
size_t pool_alloc<_Tp>::count_connected = 0;

template <typename _Tp>
size_t pool_alloc<_Tp>::max_size = 500'000;

template <typename _Tp>
_Tp *pool_alloc<_Tp>::cur = nullptr;

template <typename _Tp>
_Tp *pool_alloc<_Tp>::head = nullptr;

#endif