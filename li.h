#ifndef LI_H
#define LI_H

#include <iostream>

template <typename _Ty, typename _Ty_Alloc = std::allocator<_Ty>>
class li
{
public:
    using value_type = _Ty;
    using size_type = size_t;
    using allocator_type = _Ty_Alloc;

protected:
    struct Node
    {
        _Ty val;
        Node *prev;
        Node *next;
    };

    using reb_alloc = typename std::allocator_traits<_Ty_Alloc>::template rebind_alloc<Node>;
    reb_alloc alloc; // аллокатор
    using traits = std::allocator_traits<reb_alloc>;

    Node *head; // указатель на голову
    Node *tail; // указатель на хвост (сырая память, для итератора)
    size_t sz;

    template <bool _is_const>
    class _common_iterator
    {
    public:
        using value_type = std::conditional_t<_is_const, const Node *, Node *>;
        using pointer = value_type *;
        using difference_type = ptrdiff_t;
        using reference = value_type &;
        using iterator_category = std::bidirectional_iterator_tag;

    protected:
        value_type ptr;

    public:
        _common_iterator(value_type ptr) : ptr(ptr) {}

        _common_iterator &operator++()
        {
            ptr = ptr->next;
            return *this;
        }

        _common_iterator &operator++(int)
        {
            _common_iterator tmp = *this;
            ptr = ptr->next;
            return *this;
        }

        _common_iterator &operator--()
        {
            ptr = ptr->prev;
            return *this;
        }

        _common_iterator &operator--(int)
        {
            _common_iterator tmp = *this;
            ptr = ptr->prev;
            return *this;
        }

        value_type base() { return ptr; }

        std::conditional_t<_is_const, const _Ty &, _Ty &> operator*() { return ptr->val; }

        bool operator==(const _common_iterator &iter) const noexcept
        {
            return ptr == iter.ptr;
        }

        bool operator!=(const _common_iterator &iter) const noexcept { return ptr != iter.ptr; }

        ~_common_iterator() {}
    };

public:
    li(const allocator_type &__alloc = allocator_type()) : sz(0), alloc(__alloc)
    {
        tail = traits::allocate(alloc, 1);
        head = tail;
    }

    li(size_t __n, const _Ty &__val = _Ty(), const allocator_type &__alloc = allocator_type()) : li(__alloc)
    {
        for (size_t i = 0; i < __n; ++i)
        {
            push_back(__val);
        }
    }

    li(const std::initializer_list<_Ty> &init_li, const allocator_type &__alloc = allocator_type())
    {
        for (auto it = init_li.begin(); it != init_li.end(); ++it)
        {
            push_back(*it);
        }
    }


    void clear()
    {
        size_t size_el = sz;
        for (size_t i = 0; i < size_el; ++i)
        {
            pop_back();
        }
    }

    void push_back(const _Ty &__val)
    {
        if (head == tail)
        {
            head = traits::allocate(alloc, 1);
            traits::construct(alloc, &head->val, __val);
            head->next = tail;
            head->prev = tail;

            tail->prev = head;
            tail->next = nullptr;
        }
        else
        {
            Node *cur = traits::allocate(alloc, 1);
            traits::construct(alloc, &cur->val, __val);
            cur->next = tail;
            cur->prev = tail->prev;
            tail->prev->next = cur;
            tail->prev = cur;
        }
        ++sz;
    }

    void pop_back()
    {
        // узел, который надо удалить
        Node *tmp = tail->prev;

        if (tmp == head)
        {
            traits::destroy(alloc, &tmp->val);
            traits::deallocate(alloc, tmp, 1);

            head = tail;
        }
        else
        {
            Node *ptr_prev = tmp->prev;
            ptr_prev->next = tail;
            tail->prev = ptr_prev;

            traits::destroy(alloc, &tmp->val);
            traits::deallocate(alloc, tmp, 1);
        }
        --sz;
    }

    size_t size() const noexcept { return sz; }

    using iterator = _common_iterator<false>;

    using const_iterator = _common_iterator<true>;

    using reverse_iterator = std::reverse_iterator<iterator>;

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() const { return iterator(head); }

    iterator end() const { return iterator(tail); }

    const_iterator cbegin() const { return iterator(head); }

    const_iterator cend() const { return iterator(tail); }

    reverse_iterator rbegin() const { return reverse_iterator(tail); }

    reverse_iterator rend() const { return reverse_iterator(head); }

    const_reverse_iterator crbegin() const { return const_reverse_iterator(tail); }

    const_reverse_iterator crend() const { return const_reverse_iterator(head); }
};

#endif