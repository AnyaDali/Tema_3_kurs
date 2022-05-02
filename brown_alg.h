#ifndef BROWN_ALG_H
#define BROWN_ALG_H

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

        void clear() { set_cl.clear(); }

        ~info_node() { clear(); }

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

    using value_type = std::tuple<std::vector<info_node>,
                                  adj_colors,
                                  std::vector<int>,
                                  size_t,
                                  size_t,
                                  size_t,
                                  bool,
                                  std::vector<int>>;

    using stack_memory = std::stack<value_type>;

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

    bool __not_found(const adj_colors &adj_color_vertex, size_t index_U, int val) { return !(adj_color_vertex.contained(index_U, val)); }

    void __calc_Ui(const adj_colors &adj_color_vertex, info_node &U, size_t index_U, size_t q)
    {
        for (int i = 0; i < q + 1; ++i)
        {
            if (__not_found(adj_color_vertex, index_U, i)) // занести в множество U[i] вершины
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
                value_type &tp)
    {
        std::get<lexeme_alg::L_VEC>(tp).resize(n);
        std::get<lexeme_alg::U_SET>(tp).resize(n);
        // U.resize(n);
        std::get<lexeme_alg::C_VEC_COLOR>(tp).resize(n, -1);
        // c.resize(n, -1);
        std::get<lexeme_alg::I_ITER>(tp) = 1;
        // i = 1;
        std::get<lexeme_alg::K_BORDER>(tp) = n;
        // k = n;
        std::get<lexeme_alg::Q_BORDER>(tp) = 1;
        // q = 1;
        std::get<lexeme_alg::C_VEC_COLOR>(tp)[0] = 0;
        // c[0] = 0;
        std::get<lexeme_alg::U_SET>(tp)[0] = {};
        // U[0] = {}; // empty set
        std::get<lexeme_alg::L_VEC>(tp)[0] = 1;
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
                          std::vector<int> &c)
    {
        size_t n = g.count_of_vertex();
        while (i > 0)
        {
            if (updateU == true)
            {
                __calc_Ui(ad_cl, U[i], i, q);
            }

            if (!U[i].check_info())
            {
                --i;
                size_t k_kopy = k;
                size_t q_copy = l[i];
                bool updateU_copy = false;
                if (!st_m.empty())
                    st_m.pop();

                if (!st_m.empty())
                {
                    std::get<lexeme_alg::K_BORDER>(st_m.top()) = k_kopy;
                    // std::get<lexeme_alg::UPDETE_U>(st_m.top()) = true;
                    std::get<lexeme_alg::Q_BORDER>(st_m.top()) = q_copy;
                }

                return;
            }
            else
            {
                int j = __find_min_color_Ui_and_erase(U[i]);

                ad_cl.paint_the_vertex(i, j);
                c[i] = j;

                if (j < k)
                {
                    if (j >= q)
                    {
                        q = j + 1;
                    }

                    if (i == n - 1)
                    {
                        std::cout << q << "  " << k << std::endl;
                        int max_v = 0;
                        for (const auto &el : c)
                        {
                            max_v = std::max(max_v, el);
                        }
                        k = q = max_v + 1;
                        // std::cout << k << std::endl;
                        if (res.second > k)
                        {
                            res.second = k;
                            res.first = c;
                        }

                        j = ad_cl.get_min_index(k - 1);
                        size_t copy_k = k;

                        st_m.emplace(U, ad_cl, l, k, i, q, updateU, c);

                        for (int p = (int)n - 1; p > j - 1; --p)
                        {
                            if (!st_m.empty())
                                st_m.pop();
                        }

                        if (!st_m.empty())
                        {
                            // std::get<lexeme_alg::UPDETE_U>(st_m.top()) = true;
                            std::get<lexeme_alg::K_BORDER>(st_m.top()) = copy_k - 1;
                            std::get<lexeme_alg::Q_BORDER>(st_m.top()) = copy_k - 1;
                        }

                        return;
                    }
                    else
                    {

                        l[i] = q;
                        updateU = true;
                        //++i;
                        st_m.emplace(U, ad_cl, l, k, i + 1, q, updateU, c);
                        return;
                    }
                }
                else
                {
                    --i;
                    size_t k_copy = k;
                    size_t q_copy = l[i];
                    bool updateU_copy = false;

                    if (!st_m.empty())
                        st_m.pop();

                    if (!st_m.empty())
                    {
                        std::get<lexeme_alg::K_BORDER>(st_m.top()) = k_copy;
                        // std::get<lexeme_alg::UPDETE_U>(st_m.top()) = true;
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
    algorithm_brown(const graph_model &obj) : ref_graph_model(obj), res(std::make_pair(std::vector<int>(obj.count_of_vertex()),
                                                                                       std::numeric_limits<size_t>::max()))
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
        __init(obj.count_of_vertex(), st_m.top());
    }

    void algorithm_brown_exact()
    {
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
                             std::get<lexeme_alg::C_VEC_COLOR>(st_m.top()));
        }
    }

    std::pair<std::vector<int>, size_t> get_res() const noexcept { return res; }

    void clear()
    {
        while (!st_m.empty())
            st_m.pop();

        res.first.clear();
        res.second = 0;
    }

    ~algorithm_brown()
    {
        clear();
    }
};

#endif
