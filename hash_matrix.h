#ifndef HASH_TABLE
#define HASH_TABLE

#include "point_vertex.h"

template <std::size_t size_sq, typename _Alloc>
class hash_matrix;

template <std::size_t size_sq, typename _Alloc>
std::ostream &operator<<(std::ostream &os, hash_matrix<size_sq, _Alloc> &obj);

template <std::size_t size_sq, typename _Alloc = std::allocator<std::list<point_vertex>>>
class hash_matrix
{
public:
    //static_assert(size_sq < 2 * point_vertex::radius, "error");
    using value_type = std::list<point_vertex>;
    using allocator_type = _Alloc;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = std::size_t;
    using pointer = value_type *;
    using const_pointer = const value_type *;

protected:
    std::vector<pointer> _vecPtr;      // матрица
    std::vector<point_vertex *> __ptr; // список вершин по занесению

    size_type height, width;

    std::pair<int, int> __tranform_coord(const std::pair<int, int> &point) const { return {point.first / size_sq, point.second / size_sq}; }

    std::size_t __get_index_vector(int y, int x) const { return static_cast<std::size_t>(y * width + x); }

    point_vertex *__find_list(const std::pair<int, int> &point, const std::size_t ind) const
    {
        if (_vecPtr[ind] == nullptr)
        {
            return nullptr;
        }

        for (auto it = _vecPtr[ind]->begin(); it != _vecPtr[ind]->end(); ++it)
        {
            double deltaX = fabs(it->point.first - point.first);
            double deltaY = fabs(it->point.second - point.second);
            if (sqrt(deltaX * deltaX + deltaY * deltaY) < point_vertex::radius)
                return &(*it);
        }

        return nullptr;
    }

    std::tuple<size_t *, std::pair<int, int>> __find_list_erase(const std::pair<int, int> &point, const std::size_t ind)
    {
        if (_vecPtr[ind] == nullptr)
        {
            return std::make_tuple(nullptr, std::pair<int, int>{-1, -1});
        }

        for (auto it = _vecPtr[ind]->begin(); it != _vecPtr[ind]->end(); ++it)
        {
            double deltaX = fabs(it->point.first - point.first);
            double deltaY = fabs(it->point.second - point.second);
            if (sqrt(deltaX * deltaX + deltaY * deltaY) < point_vertex::radius)
            {
                _vecPtr[ind]->erase(it);
                point_vertex::index -= 1;
                return std::make_tuple(new size_t(it->indexArray), it->point);
            }
        }

        return std::make_tuple(nullptr, std::pair<int, int>{-1, -1});
    }

    void __change_indexes(size_t ind)
    {
        for (auto it = __ptr.begin() + ind; it != __ptr.end(); ++it)
        {
            (*it)->indexArray -= 1;
        }
    }

public:
    hash_matrix(size_type height, size_type width) : height(height / size_sq), width(width / size_sq)
    {
        _vecPtr.resize(this->height * this->width, nullptr);
    }

    std::pair<size_type, size_type> get_size() const { return {height, width}; }

    // О(1) (mean)
    void emplace(const std::pair<int, int> &point)
    {
        auto p = __tranform_coord(point);
        std::size_t ind = __get_index_vector(p.first, p.second);
        if (_vecPtr[ind] == nullptr)
        {
            _vecPtr[ind] = new value_type();
        }
        _vecPtr[ind]->push_back(point);

        __ptr.push_back(&_vecPtr[ind]->back());
    }

    void erase_ptr(size_t ind)
    {
        __ptr.erase(__ptr.begin() + ind);
        __change_indexes(ind);
    }

    std::tuple<size_t *, std::pair<int, int>> find_and_erase(const std::pair<int, int> &point)
    {
        auto p = __tranform_coord(point);
        std::size_t ind = __get_index_vector(p.first, p.second);
        auto ptr = __find_list_erase(point, ind);
        auto copy_point = point;
        bool flag1 = false, flag2 = false;
        if (std::get<0>(ptr) == nullptr)
        {
            if (point.first % size_sq < point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first - 10, point.second});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = __find_list_erase(point, ind);
                copy_point.first -= 10;
                flag1 = true;
            }
            else if (point.first % size_sq > size_sq - point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first + 10, point.second});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = __find_list_erase(point, ind);
                copy_point.first += 10;
                flag1 = true;
            }
        }
        if (std::get<0>(ptr) == nullptr)
        {
            if (point.second % size_sq < point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first, point.second - 10});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = __find_list_erase(point, ind);
                copy_point.second -= 10;
                flag2 = true;
            }
            else if (point.second % size_sq > size_sq - point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first, point.second + 10});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = __find_list_erase(point, ind);
                copy_point.second += 10;
                flag2 = true;
            }
        }
        if (std::get<0>(ptr) == nullptr)
        {
            if (flag1 && flag2)
            {
                auto x1 = __tranform_coord(copy_point);
                ind = __get_index_vector(x1.first, x1.second);
                ptr = __find_list_erase(point, ind);
            }
        }

        // result
        if (std::get<0>(ptr) != nullptr)
        {
            erase_ptr(*std::get<0>(ptr));
        }
        return ptr;
    }

    void clear()
    {

        for (auto &ptr : _vecPtr)
        {
            if (ptr != nullptr)
            {
                ptr->clear();
                delete ptr;
                ptr = nullptr;
            }
        }

        __ptr.clear();

        point_vertex::index = 0;
    }

    point_vertex *exact_location(const std::pair<int, int> &point)
    {
        auto p = __tranform_coord(point);
        std::size_t ind = __get_index_vector(p.first, p.second);

        for (std::list<point_vertex>::iterator it = _vecPtr[ind]->begin(); it != _vecPtr[ind]->end(); ++it)
        {
            if (it->point.first == point.first && it->point.second == point.second)
                return &(*it);
        }
        return nullptr;
    }

    ~hash_matrix()
    {
        std::cout << "~hash_matrix\n";
        clear();
        if (!_vecPtr.empty())
        {
            _vecPtr.clear();
        }
    }

    point_vertex *find(const std::pair<int, int> &point)
    {
        auto p = __tranform_coord(point);
        std::size_t ind = __get_index_vector(p.first, p.second);
        auto ptr = hash_matrix<size_sq, _Alloc>::__find_list(point, ind);
        auto copy_point = point;
        bool flag1 = false, flag2 = false;
        if (ptr == nullptr)
        {
            if (point.first % size_sq < point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first - 10, point.second});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = __find_list(point, ind);
                copy_point.first -= 10;
                flag1 = true;
            }
            else if (point.first % size_sq > size_sq - point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first + 10, point.second});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = __find_list(point, ind);
                copy_point.first += 10;
                flag1 = true;
            }
        }
        if (ptr == nullptr)
        {
            if (point.second % size_sq < point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first, point.second - 10});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = __find_list(point, ind);
                copy_point.second -= 10;
                flag2 = true;
            }
            else if (point.second % size_sq > size_sq - point_vertex::radius)
            {
                auto x1 = __tranform_coord({point.first, point.second + 10});
                ind = __get_index_vector(x1.first, x1.second);
                ptr = __find_list(point, ind);
                copy_point.second += 10;
                flag2 = true;
            }
        }
        if (ptr == nullptr)
        {
            if (flag1 && flag2)
            {
                auto x1 = __tranform_coord(copy_point);
                ind = __get_index_vector(x1.first, x1.second);
                ptr = __find_list(point, ind);
            }
        }

        return ptr;
    }

    void print_li() const
    {
        for (const auto el : __ptr)
        {
            std::cout << *el << std::endl;
        }
        std::cout << point_vertex::index << '\n';
    }

    void print() const
    {
        for (std::size_t i = 0; i < _vecPtr.size(); ++i)
        {
            if (_vecPtr[i] == nullptr)
            {
                std::cout << "{}";
            }
            else
            {
                std::cout << "{";

                for (auto it = _vecPtr[i]->begin(); it != _vecPtr[i]->end(); ++it)
                {
                    std::cout << *it << " ";
                }
                std::cout << "}";
            }
        }
        std::cout << '\n';
    }
};

#endif