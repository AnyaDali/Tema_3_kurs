#include "graph_model.h"

struct info;
class adj_colors;

std::ostream &operator<<(std::ostream &os, const std::vector<int> &vec)
{
    for (auto el : vec)
    {
        os << el << " ";
    }
    return os;
}

using stack_memory = std::stack<std::tuple<std::vector<info>,
                                           adj_colors,
                                           std::vector<int>,
                                           size_t,
                                           size_t,
                                           size_t,
                                           bool,
                                           std::vector<int>>>;

enum lexeme_alg
{
    U_SET,
    ADJ_COLORS,
    L_VEC,
    K_BORDER,
    I_ITER,
    Q_BORDER,
    UPDETE_U,
    C_VEC_COLOR
};

struct info
{
    std::map<int, bool> set_cl;

    info() : set_cl() {}

    info(const info &obj) : set_cl(obj.set_cl) {}

    info(std::initializer_list<std::pair<int, bool>> li_init)
    {
        for (const auto &el : li_init)
        {
            set_cl.emplace(el);
        }
    }

    void emplace(int &_val, bool f) { set_cl.emplace(_val, f); }

    void used(size_t ind) { set_cl[ind] = false; }

    bool empty() const noexcept { return set_cl.empty(); }

    ~info() { set_cl.clear(); }

    info &operator=(const info &obj)
    {
        if (this == &obj)
            return *this;
        else
            set_cl = obj.set_cl;
        return *this;
    }

    info &operator=(const std::initializer_list<std::pair<int, bool>> &li_init)
    {
        for (const auto &el : li_init)
        {
            set_cl.emplace(el);
        }
    }

    bool check_info()
    {
        for (const auto &el : set_cl)
        {
            if (el.second)
                return true;
        }
        return false;
    }

    friend std::ostream &operator<<(std::ostream &os, info &obj);
};

std::ostream &operator<<(std::ostream &os, info &obj)
{
    os << "{ ";
    for (auto &el : obj.set_cl)
    {
        os << "(" << el.first << " " << el.second << ")";
    }
    os << "} ";
}

std::ostream &operator<<(std::ostream &os, std::vector<info> &vec)
{
    for (auto el : vec)
    {
        os << el << " ";
    }
    return os;
}

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

    ~adj_colors()
    {
        for (size_t i = 0; i < _vec_ptr.size(); ++i)
        {
            delete _vec_ptr[i];
        }
    }
};

std::ostream &operator<<(std::ostream &os, adj_colors &obj)
{
    size_t i = 0;
    for (auto &li : obj.adj_list)
    {
        os << i << ": { ";
        for (auto &el : li)
        {
            os << "(" << *el.first << ", " << el.second << ") ";
        }
        os << "}\n";
        ++i;
    }
    return os;
}

bool not_found(const adj_colors &adj_color_vertex, size_t index_U, int val) { return !(adj_color_vertex.contained(index_U, val)); }

void calc_Ui(const adj_colors &adj_color_vertex, info &U, size_t index_U, size_t q)
{
    for (int i = 0; i < q + 1; ++i)
    {
        if (not_found(adj_color_vertex, index_U, i)) // занести в множество U[i] вершины
        {
            U.emplace(i, true);
        }
        else
        {
            U.emplace(i, false);
        }
    }
}

int find_min_color_Ui_and_erase(info &Ui)
{
    for (auto &el : Ui.set_cl)
    {
        if (el.second == true)
        {
            el.second = false;
            return el.first;
        }
    }
    return -1;
}

void init(size_t n,
          stack_memory &st_memory)
{
    auto &tp = st_memory.top();
    std::get<lexeme_alg::L_VEC>(tp).resize(n);
    std::get<lexeme_alg::U_SET>(tp).resize(n);
    // U.resize(n);
    std::get<lexeme_alg::C_VEC_COLOR>(tp).resize(n, -1);
    // c.resize(n, -1);
    std::get<lexeme_alg::I_ITER>(tp) = 1;
    // i = 1;
    std::get<lexeme_alg::K_BORDER>(tp) = n - 1;
    // k = n;
    std::get<lexeme_alg::Q_BORDER>(tp) = 1;
    // q = 1;
    std::get<lexeme_alg::C_VEC_COLOR>(tp)[0] = 0;
    // c[0] = 0;
    std::get<lexeme_alg::U_SET>(tp)[0] = {};
    // U[0] = {}; // empty set
    std::get<lexeme_alg::L_VEC>(tp)[0] = 0;
    // l[0] = 0;
    std::get<lexeme_alg::ADJ_COLORS>(tp).paint_the_vertex(0, 0);
    // ad_cl.paint_the_vertex(0, 0);
    std::get<lexeme_alg::UPDETE_U>(tp) = true;
    // updateU = true;
}

void alg_brown(stack_memory &st_m,
               const graph_model &g, /* предсталение вершин графа в виде списков смежности */
               std::vector<info> &U, /* множество цветов для вершин */
               adj_colors &ad_cl,    /* смежные цвета */
               std::vector<int> &l,
               size_t &k, /* верхняя граница */
               size_t &i, /* итерация */
               size_t &q, /* нижняя граница */
               bool &updateU,
               std::vector<int> &c, /*вектор цветов*/
               std::vector<int> &vec_color_colution,
               std::size_t &min_color)
{
    size_t n = g.count_of_vertex();
    while (i > 0)
    {

        if (updateU == true)
        {
            calc_Ui(ad_cl, U[i], i, q);
            /* std::cout << "set = {";
            // for (auto el : U[i].set_cl)
            //{
            //   std::cout << "(" << el.first << " " << el.second << ") ";
            // }
            // std::cout << '\n';*/
        }

        if (!U[i].check_info())
        {
            --i;
            size_t k_kopy = k;
            size_t q_copy = l[i];
            bool updateU_copy = false;
            if (!st_m.empty())
                st_m.pop(); // вернуться на решение вверх
            if (!st_m.empty())
            {
                std::get<lexeme_alg::K_BORDER>(st_m.top()) = k_kopy;
                std::get<lexeme_alg::UPDETE_U>(st_m.top()) = updateU_copy;
                std::get<lexeme_alg::Q_BORDER>(st_m.top()) = q_copy;
            }

            return;
        }
        else
        {
            int j = find_min_color_Ui_and_erase(U[i]);
            ad_cl.paint_the_vertex(i, j);
            // выбрать j, принаждлежащее U[i],
            // такое, что это минимальное возможное значение
            // удалить цвет j из множества U[i]
            c[i] = j;

            // и задаем цвет j вершине v[i]

            if (j < k)
            {
                if (j >= q)
                {
                    q = j + 1;
                }

                if (i == n - 1)
                {

                    int max_v = 0;
                    for (const auto &el : c)
                    {
                        max_v = std::max(max_v, el);
                    }
                    k = q = max_v + 1;

                    if (min_color > k)
                    {
                        min_color = k;
                        vec_color_colution = c;
                    }
                    std::cout << "vec c = " << c << "k = " << k << std::endl;
                    /* запомнить текущее решение и установить k = q */
                    /* найти наименьший номер j такой, что цвет v[j] = k */
                    j = ad_cl.get_min_index(k - 1);
                    size_t copy_k = k;
                    if (j == -1)
                    {
                        std::cout << "j == -1" << std::endl;
                        if (!st_m.empty())
                        {
                            st_m.pop();
                        }
                        if (!st_m.empty())
                        {
                            std::get<lexeme_alg::K_BORDER>(st_m.top()) = copy_k;
                        }
                        return;
                    }

                    for (int p = (int)n; p > j - 1; --p)
                    {
                        if (!st_m.empty())
                            st_m.pop();
                    }
                    if (!st_m.empty())
                    {
                        std::get<lexeme_alg::UPDETE_U>(st_m.top()) = false;
                        std::get<lexeme_alg::K_BORDER>(st_m.top()) = copy_k - 1;
                        std::get<lexeme_alg::Q_BORDER>(st_m.top()) = copy_k - 1;
                    }

                    // i = j - 1;
                    // q = k - 1;
                    // updateU = false;
                    return;
                }
                else
                {
                    l[i] = q;
                    st_m.emplace(std::make_tuple(U, ad_cl, l, k, i, q, updateU, c));
                    ++i; // новая вершиная, выбранная для раскраски
                    updateU = true;
                }
            }
            else
            {
                --i;
                size_t k_kopy = k;
                size_t q_copy = l[i];
                bool updateU_copy = false;
                // вернуться на решение вверх
                if (!st_m.empty())
                    st_m.pop();

                if (!st_m.empty())
                {
                    std::get<lexeme_alg::K_BORDER>(st_m.top()) = k_kopy;
                    std::get<lexeme_alg::UPDETE_U>(st_m.top()) = updateU_copy;
                    std::get<lexeme_alg::Q_BORDER>(st_m.top()) = q_copy;
                }
                return;
            }
        }
    }
}

std::pair<std::vector<int>, size_t> alg_main(const graph_model &g /* предсталение вершин графа в виде списков смежности */)
{
    std::vector<info> U; /* множество цветов для вершин */
    adj_colors ad_cl(g); /* смежные цвета */
    std::vector<int> l;
    size_t k; /* верхняя граница */
    size_t i; /* итерация */
    size_t q; /* нижняя граница */
    bool updateU;
    std::vector<int> c; /*вектор цветов*/
    stack_memory st_memory;
    st_memory.emplace(std::make_tuple(U, ad_cl, l, k, i, q, updateU, c));

    init(g.count_of_vertex(), st_memory);
    std::cout << std::get<lexeme_alg::ADJ_COLORS>(st_memory.top()) << std::endl;

    std::vector<int> vec_solution(g.count_of_vertex());

    size_t min_res = UINT32_MAX;

    while (st_memory.size() != 0)
    {
        alg_brown(st_memory, g,
                  std::get<lexeme_alg::U_SET>(st_memory.top()),
                  std::get<lexeme_alg::ADJ_COLORS>(st_memory.top()),
                  std::get<lexeme_alg::L_VEC>(st_memory.top()),
                  std::get<lexeme_alg::K_BORDER>(st_memory.top()),
                  std::get<lexeme_alg::I_ITER>(st_memory.top()),
                  std::get<lexeme_alg::Q_BORDER>(st_memory.top()),
                  std::get<lexeme_alg::UPDETE_U>(st_memory.top()),
                  std::get<lexeme_alg::C_VEC_COLOR>(st_memory.top()), vec_solution, min_res);
    }
    std::cout << vec_solution << std::endl;
    std::cout << min_res << std::endl;
    return {vec_solution, min_res};
}
/*
int main()
{
    graph_model obj;
    for (size_t i = 0; i < 8; ++i)
    {
        obj.create_vertex();
    }

    obj.add(0, 1);
    obj.add(0, 2);
    obj.add(0, 6);

    obj.add(1, 5);
    obj.add(1, 7);

    obj.add(2, 3);
    obj.add(2, 4);

    obj.add(3, 4);
    obj.add(3, 5);

    obj.add(4, 5);
    alg_main(obj);
}
*/
/*
    obj.add(0, 5);

    obj.add(1, 0);
    obj.add(1, 2);
    obj.add(1, 3);
    obj.add(1, 4);
    obj.add(1, 5);

    obj.add(2, 3);

    obj.add(3, 4);
    obj.add(4, 5);

// std::cout << obj << std::endl;
alg_main(obj);
return 0;
}
*/
