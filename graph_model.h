#ifndef GRAPH_MODEL
#define GRAPH_MODEL
#include "file_include.h"

class graph_model
{
protected:
    std::vector<std::list<int *>> adj_lists;
    std::vector<int *> vec_ptr;

    void __erase_ptr(size_t ind)
    {
        delete vec_ptr[ind];
        vec_ptr.erase(vec_ptr.begin() + ind);
        for (size_t i = ind; i < vec_ptr.size(); ++i)
        {
            *vec_ptr[i] -= 1;
        }
    }

public:
    std::list<int *> &operator[](size_t ind) { return adj_lists[ind]; }

    const std::list<int *> &operator[](size_t ind) const { return adj_lists[ind]; }

    graph_model(const graph_model &copy) : adj_lists(copy.adj_lists) {}

    graph_model(graph_model &&obj) noexcept { adj_lists = std::move(obj.adj_lists); }

    graph_model &operator=(const graph_model &copy)
    {
        if (this == &copy)
            return *this;
        adj_lists = copy.adj_lists;
        return *this;
    }

    graph_model &operator=(graph_model &&obj) noexcept
    {
        adj_lists = std::move(obj.adj_lists);
        return *this;
    }

    graph_model() {}

    void create_vertex()
    {
        vec_ptr.emplace_back(new int(static_cast<int>(vec_ptr.size())));
        adj_lists.emplace_back(std::move(std::list<int *>()));
    }

    void add(int __v1, int __v2)
    {
        adj_lists[__v1].emplace_back(vec_ptr[__v2]);
        adj_lists[__v2].emplace_back(vec_ptr[__v1]);
    }

    void erase_vertex(int __v1)
    {
        adj_lists.erase(adj_lists.begin() + __v1);
        for (auto &li : adj_lists)
        {
            auto it = std::find_if(li.begin(), li.end(), [&](const auto &el)
                                   { return (__v1 == *el) ? true : false; });
            if (it != li.end())
            {
                li.erase(it);
            }
        }

        __erase_ptr(__v1);
    }

    void erase_way(int __v1, int __v2)
    {
        auto ptr1 = std::find_if(adj_lists[__v1].begin(), adj_lists[__v1].end(), [&](const auto &el)
                                 { return (__v2 == *el) ? true : false; });

        adj_lists[__v1].erase(ptr1);

        auto ptr2 = std::find_if(adj_lists[__v2].begin(), adj_lists[__v2].end(), [&](const auto &el)
                                 { return (__v1 == *el) ? true : false; });

        adj_lists[__v2].erase(ptr2);
    }

    size_t count_of_vertex() const noexcept { return adj_lists.size(); }

    size_t get_degress_of_vertex(size_t i) const noexcept { return adj_lists[i].size(); }

    friend std::ostream &operator<<(std::ostream &os, const graph_model &obj);

    void clear()
    {
        for (auto &el : vec_ptr)
        {
            delete el;
        }
        vec_ptr.clear();

        adj_lists.clear();
    }

    ~graph_model()
    {
        clear();
    }
};

std::ostream &operator<<(std::ostream &os, const graph_model &obj)
{
    for (const auto &li : obj.adj_lists)
    {
        os << "{ ";
        for (const auto &el : li)
        {
            os << *el << " ";
        }
        os << "}\n";
    }
    return os;
}

#endif
