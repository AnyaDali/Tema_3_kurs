#ifndef ADJ_COLORS_H
#define ADJ_COLORS_H

#include "graph_model.h"

class adj_colors
{
protected:
    std::vector<std::list<std::pair<int *, int>>> adj_list;
    std::vector<int *> _vec_ptr;

    void __init_ptr(size_t n)
    {
        _vec_ptr.resize(n);
        for (size_t i = 0; i < n; ++i)
        {
            _vec_ptr[i] = new int(-1);
        }
    }

public:
    friend std::ostream &operator<<(std::ostream &os, adj_colors &obj);
    adj_colors() : adj_list(), _vec_ptr() {}

    adj_colors(const graph_model &obj)
    {
        __init_ptr(obj.count_of_vertex());
        adj_list.resize(obj.count_of_vertex());
        for (size_t i = 0; i < obj.count_of_vertex(); ++i)
        {
            for (auto &el : obj[i])
            {
                adj_list[i].emplace_back(std::pair<int *, int>{_vec_ptr[*el], *el});
            }
        }
    }

    adj_colors(const adj_colors &obj)
    {
        _vec_ptr.resize(obj._vec_ptr.size());
        for (size_t i = 0; i < _vec_ptr.size(); ++i)
        {
            _vec_ptr[i] = new int(*obj._vec_ptr[i]);
        }

        adj_list.resize(obj.adj_list.size());

        for (size_t i = 0; i < adj_list.size(); ++i)
        {
            for (auto &el : obj.adj_list[i])
            {
                adj_list[i].emplace_back(std::pair<int *, int>{_vec_ptr[el.second], el.second});
            }
        }
    }

    adj_colors &operator=(const adj_colors &obj)
    {
        for (const auto &el : obj._vec_ptr)
        {
            delete el;
        }

        adj_list.clear();
        _vec_ptr.clear();

        _vec_ptr.resize(obj._vec_ptr.size());
        for (size_t i = 0; i < _vec_ptr.size(); ++i)
        {
            _vec_ptr[i] = new int(*obj._vec_ptr[i]);
        }

        adj_list.resize(obj.adj_list.size());

        for (size_t i = 0; i < obj.adj_list.size(); ++i)
        {
            for (auto &el : obj[i])
            {
                adj_list[i].emplace_back(std::pair<int *, int>{_vec_ptr[el.second], el.second});
            }
        }
        return *this;
    }

    void paint_the_vertex(size_t i, int val)
    {
        *(_vec_ptr[i]) = val;
    }

    int get_free_index_color(size_t i)
    {
        for (int j = 0; true; ++j)
        {
            auto it = std::find_if(adj_list[i].begin(), adj_list[i].end(), [&](const auto &el)
                                   { return (*el.first == j) ? true : false; });

            if (it == adj_list[i].end())
            {
                return j;
            }
        }
    }

    const std::pair<int *, int> *find(size_t i, int &__val) const
    {
        auto it = std::find_if(adj_list[i].begin(), adj_list[i].end(), [&](const auto &el)
                               { return (*el.first == __val) ? true : false; });

        if (it == adj_list[i].end())
            return nullptr;
        else
            return &(*it);
    }

    bool contained(size_t i, int &__val) const
    {
        auto it = std::find_if(adj_list[i].begin(), adj_list[i].end(), [&](const auto &el)
                               { return (*el.first == __val) ? true : false; });

        if (it == adj_list[i].end())
            return false;
        else
            return true;
    }

    const std::list<std::pair<int *, int>> &operator[](size_t ind) const noexcept { return adj_list[ind]; }

    const std::vector<int *> &get_li_color() const noexcept { return _vec_ptr; }

    int get_min_index(size_t __key)
    {
        for (size_t i = 0; i < _vec_ptr.size(); ++i)
        {
            if (*_vec_ptr[i] == __key)
                return i;
        }
        return -1;
    }

    void clear()
    {
        for (auto &el : _vec_ptr)
        {
            delete el;
        }

        _vec_ptr.clear();

        adj_list.clear();
    }

    ~adj_colors()
    {
        clear();
    }
};

#endif