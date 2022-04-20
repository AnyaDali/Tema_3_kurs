#include "graph_model.h"
#include "adj_colors.h"

class algorithm_brown
{
protected:
    struct info_node
    {

        std::map<int, bool> set_cl;

        info_node() : set_cl() {}

        info_node(const info_node &obj) : set_cl(obj.set_cl) {}

        info_node(const std::initializer_list<std::pair<int, bool>> &li_init)
        {
            for (const auto &el : li_init)
            {
                set_cl.emplace(el);
            }
        }

        void emplace(int &_val, bool f) { set_cl.emplace(_val, f); }

        void used(size_t ind) { set_cl[ind] = false; }

        bool empty() const noexcept { return set_cl.empty(); }

        ~info_node() { set_cl.clear(); }

        info_node &operator=(const info_node &obj)
        {
            if (this == &obj)
                return *this;
            else
                set_cl = obj.set_cl;
            return *this;
        }

        info_node &operator=(const std::initializer_list<std::pair<int, bool>> &li_init)
        {
            for (const auto &el : li_init)
            {
                set_cl.emplace(el);
            }
            return *this;
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
    };

    using stack_memory = std::stack<std::tuple<std::vector<info_node>,
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

    bool not_found(const adj_colors &adj_color_vertex, size_t index_U, int val) { return !(adj_color_vertex.contained(index_U, val)); }

    void __calc_Ui(const adj_colors &adj_color_vertex, info_node &U, size_t index_U, size_t q)
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

    int __find_min_color_Ui_and_erase(info_node &Ui)
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

    void __init(size_t n,
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

    void __procedue_brown(stack_memory &st_m,
                          const graph_model &g,      /* предсталение вершин графа в виде списков смежности */
                          std::vector<info_node> &U, /* множество цветов для вершин */
                          adj_colors &ad_cl,         /* смежные цвета */
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
                __calc_Ui(ad_cl, U[i], i, q);
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
                int j = __find_min_color_Ui_and_erase(U[i]);
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

    const graph_model &ref_graph_model;
    stack_memory st_m;

    std::pair<std::vector<int>, size_t> res;

public:
    algorithm_brown(const graph_model &obj) : ref_graph_model(obj) : res({}, 0)
    {
        std::vector<info_node> U; /* множество цветов для вершин */
        adj_colors ad_cl(obj);    /* смежные цвета */
        std::vector<int> l;
        size_t k; /* верхняя граница */
        size_t i; /* итерация */
        size_t q; /* нижняя граница */
        bool updateU;
        std::vector<int> c; /*вектор цветов*/
        st_m.emplace(std::make_tuple(U, ad_cl, l, k, i, q, updateU, c));
        __init(obj.count_of_vertex(), st_m);
    }

    void algorithm_brown_exact()
    {
        std::vector<int> vec_solution(ref_graph_model.count_of_vertex());

        size_t min_res = UINT32_MAX;

        while (st_m.size() != 0)
        {
            __procedue_brown(st_m, ref_graph_model,
                             std::get<lexeme_alg::U_SET>(st_m.top()),
                             std::get<lexeme_alg::ADJ_COLORS>(st_m.top()),
                             std::get<lexeme_alg::L_VEC>(st_m.top()),
                             std::get<lexeme_alg::K_BORDER>(st_m.top()),
                             std::get<lexeme_alg::I_ITER>(st_m.top()),
                             std::get<lexeme_alg::Q_BORDER>(st_m.top()),
                             std::get<lexeme_alg::UPDETE_U>(st_m.top()),
                             std::get<lexeme_alg::C_VEC_COLOR>(st_m.top()), vec_solution, min_res);
        }
    }

    std::pair<std::vector<int>, size_t> get_res() const noexcept { return res; }
};

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

    algorithm_brown al_b(obj);
    al_b.algorithm_brown_exact();

    return 0;
}

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
