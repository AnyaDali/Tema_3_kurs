#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

template <typename _Ty, std::size_t n, std::size_t m>
class matrix;

template <typename _Ty, std::size_t n, std::size_t m>
std::ostream &operator<<(std::ostream &os, matrix<_Ty, n, m> &mx);

template <typename _Ty, std::size_t n, std::size_t m>
std::istream &operator>>(std::istream &is, matrix<_Ty, n, m> &mx);

template <typename _Ty, std::size_t n, std::size_t m>
class matrix
{
protected:
    std::vector<_Ty> _ptr;

    void __scalar_product(matrix<_Ty, m, n> &mx, std::size_t i_ind, std::size_t j_ind, _Ty &el)
    {
        for (std::size_t i = 0; i < m; ++i)
        {
            el += (*(this->operator[](i_ind) + i)) * (mx[i][j_ind]);
        }
    }

    static void __clac_line(matrix<_Ty, n, m> &mx_a, matrix<_Ty, m, n> &mx_b, matrix<_Ty, n, m> &res, std::size_t i_ind)
    {
        for (std::size_t i = 0; i < m; ++i)
        {
            for (std::size_t j = 0; j < m; ++j)
            {
                res[i_ind][i] += (mx_a[i_ind][j] * (mx_b[j][i]));
            }
        }
    }
    
        static void __clac_mult_interval(matrix<_Ty, n, m> &mx_a, matrix<_Ty, m, n> &mx_b, matrix<_Ty, n, m> &res,
                                    std::size_t left_border, std::size_t right_border)
        {
            for (std::size_t k = left_border; k < right_border; ++k)
            {
                for (std::size_t i = 0; i < m; ++i)
                {
                    for (std::size_t j = 0; j < m; ++j)
                    {
                        res[k][i] += (mx_a[k][j] * (mx_b[j][i]));
                    }
                }
            }
        }
    
    static void __calc_sum_interval(matrix<_Ty, n, m> &mx_a, matrix<_Ty, n, m> &mx_b, matrix<_Ty, n, m> &res,
                                std::size_t left_border, std::size_t right_border)
    {
        for (std::size_t i = left_border; i < right_border; ++i)
        {
            for (std::size_t j = 0; j < m; ++j)
            {
                res[i][j] += (mx_a[i][j] + mx_b[i][j]);
            }
        }
    }

public:
    using value_type = _Ty;

    matrix() { _ptr.resize(n * m, _Ty()); }

    matrix(const std::initializer_list<_Ty> &li_val) : matrix()
    {
        std::size_t i = 0;
        for (auto it = li_val.begin(); it != li_val.end(); ++it)
        {
            _ptr[i] = *it;
            ++i;
        }
    }

    matrix(const matrix<_Ty, n, m> &mx) { this->_ptr = mx._ptr; }

    matrix<_Ty, n, m> &operator=(const matrix<_Ty, n, m> &mx)
    {
        if (this == &mx)
            return *this;

        this->_ptr = mx._ptr;
        return *this;
    }

    _Ty *operator[](std::size_t index) { return (_ptr.data() + n * index); }

    _Ty *operator[](std::size_t index) const { return (_ptr.data() + n * index); }

    matrix<_Ty, n, m> operator*(matrix<_Ty, m, n> &mx)
    {
        matrix<_Ty, n, m> res;

        for (std::size_t i = 0; i < n; ++i)
        {
            for (std::size_t j = 0; j < m; ++j)
            {
                __scalar_product(mx, i, j, res[i][j]);
            }
        }

        return res;
    }

    /*
        static matrix<_Ty, n, m> mult_thread(matrix<_Ty, n, m> &mx_a, matrix<_Ty, m, n> &mx_b)
        {
            matrix<_Ty, n, m> res;
            std::list<std::thread *> li_thread;

            for (std::size_t i = 0; i < n; ++i)
            {
                li_thread.push_back(new std::thread(&matrix<_Ty, n, m>::__clac_line, std::ref(mx_a), std::ref(mx_b), std::ref(res), i));
            }

            for (auto it = li_thread.begin(); it != li_thread.end(); ++it)
            {
                (*it)->join();
                delete *it;
            }
            return res;
        }
    */

    static matrix<_Ty, n, m> mult_thread(matrix<_Ty, n, m> &mx_a, matrix<_Ty, m, n> &mx_b)
    {
        matrix<_Ty, n, m> res;

        std::size_t count_core = std::thread::hardware_concurrency();
        std::vector<std::thread> vec_th;
        vec_th.reserve(count_core);
        std::size_t step = n < count_core ? 1ULL : n / count_core;
        for (std::size_t i = 0; i < n; i += step)
        {
            std::thread ptr(matrix<_Ty, n, m>::__clac_mult_interval, std::ref(mx_a), std::ref(mx_b), std::ref(res), i,
                            (i + step) < n ? i + step : n);
            vec_th.emplace_back(std::move(ptr));
        }
        std::for_each(std::begin(vec_th), std::end(vec_th), [](std::thread &th)
                      { th.join(); });

        vec_th.clear();

        return res;
    }

    static matrix<_Ty, n, m> plus_thread(matrix<_Ty, n, m> &mx_a, matrix<_Ty, n, m> &mx_b)
    {
        matrix<_Ty, n, m> res;

        std::size_t count_core = std::thread::hardware_concurrency();
        std::vector<std::thread> vec_th;
        vec_th.reserve(count_core);
        std::size_t step = n < count_core ? 1ULL : n / count_core;

        for (std::size_t i = 0; i < n; i += step)
        {
            std::thread ptr(matrix<_Ty, n, m>::__calc_sum_interval, std::ref(mx_a), std::ref(mx_b), std::ref(res),
                            i, (i + step) < n ? i + step : n);
            vec_th.emplace_back(std::move(ptr));
        }

        std::for_each(std::begin(vec_th), std::end(vec_th), [](std::thread &th)
                      { th.join(); });

        vec_th.clear();

        return res;
    }

    ~matrix()
    {
        _ptr.clear();
    }

    friend std::ostream &operator<<<_Ty, n, m>(std::ostream &os, matrix<_Ty, n, m> &mx);
    friend std::istream &operator>><_Ty, n, m>(std::istream &is, matrix<_Ty, n, m> &mx);
};

template <typename _Ty, std::size_t n, std::size_t m>
std::ostream &operator<<(std::ostream &os, matrix<_Ty, n, m> &mx)
{
    for (std::size_t i = 0; i < n; ++i)
    {
        for (std::size_t j = 0; j < m; ++j)
        {
            os << mx[i][j] << " ";
        }
        if (i + 1 < n)
        {
            os << '\n';
        }
    }
    return os;
}

template <typename _Ty, std::size_t n, std::size_t m>
std::istream &operator>>(std::istream &is, matrix<_Ty, n, m> &mx)
{
    for (std::size_t i = 0; i < n; ++i)
    {
        for (std::size_t j = 0; j < m; ++j)
        {
            is >> mx[i][j];
        }
    }
    return is;
}

int main()
{
    matrix<int, 3, 3> mx1 = {1, 2, 3,
                             4, 5, 6,
                             7, 8, 9};

    matrix<int, 3, 3> mx2 = {2, 2, 2,
                             2, 2, 2,
                             2, 2, 2};

    matrix<int, 3, 3> mx3 = matrix<int, 3, 3>::mult_thread(mx1, mx2);
    matrix<int, 3, 3> mx4 = matrix<int, 3, 3>::plus_thread(mx1, mx2);
    std::cout << "mult mx1 mx2:\n";
    std::cout << mx3 << std::endl;
    std::cout << "plus mx1 mx2:\n";
    std::cout << mx4 << std::endl;

    return 0;
}