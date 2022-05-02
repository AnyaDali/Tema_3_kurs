#ifndef DSATUR_ALG_H
#define DSATUR_ALG_H

#include "graph_model.h"

class DSATUR_alg
{
protected:
    struct node_info
    {
        int sat;
        int deg;
        int vertex;
    };

    struct comparator
    {
        bool operator()(const node_info &lhs,
                        const node_info &rhs) const
        {
            return std::tuple<int, int, int>(lhs.sat, lhs.deg, lhs.vertex) > std::tuple<int, int, int>(rhs.sat, rhs.deg, rhs.vertex);
        }
    };

    const graph_model &ref_graph_model;

    std::pair<std::vector<int>, size_t> res;

public:
    DSATUR_alg(const graph_model &obj) : ref_graph_model(obj), res(std::make_pair(std::vector<int>(obj.count_of_vertex(), -1), 0)) {}

    void DSATUR_algorithm()
    {
        auto &color_vec = res.first;
        auto &min_number_color = res.second;

        std::size_t n = ref_graph_model.count_of_vertex();

        int u, i;
        std::vector<bool> used(n, false);                       // использованная вершина
        std::vector<int> d(n);                                  // d(i) кол-во смежных вершин i-ой вершины
        std::vector<std::set<int>> adjCols(n, std::set<int>()); // множество цветов каждой вершины
        std::set<node_info, comparator> Q;                      // упорядоченное множество вершин по насыщенночти
        auto maxPtr = Q.begin();
        for (u = 0; u < n; ++u)
        {
            d[u] = ref_graph_model.get_degress_of_vertex(u);
            Q.emplace(std::move(node_info{0, d[u], u})); // заполение множества
        }

        while (!Q.empty())
        {
            maxPtr = Q.begin();
            u = (*maxPtr).vertex;

            Q.erase(maxPtr);

            for (auto &v : ref_graph_model[u])
                if (color_vec[*v] != -1)
                    used[color_vec[*v]] = true;

            for (i = 0; i < used.size(); i++)
                if (used[i] == false)
                    break;

            for (auto &v : ref_graph_model[u])
                if (color_vec[*v] != -1)
                    used[color_vec[*v]] = false;

            color_vec[u] = i;

            for (auto &v : ref_graph_model[u])
            {
                if (color_vec[*v] == -1)
                {
                    Q.erase(
                        {static_cast<int>(adjCols[*v].size()),
                         d[*v], *v});
                    adjCols[*v].insert(i);
                    d[*v]--;
                    Q.emplace(node_info{
                        static_cast<int>(adjCols[*v].size()),
                        d[*v], *v});
                    min_number_color = std::max(min_number_color, adjCols[*v].size() + 1);
                }
            }
        }
    }

    const std::pair<std::vector<int>, size_t> &get_res() const noexcept { return res; }

    void clear()
    {
        res.first.clear();
        res.second = 0;
    }

    ~DSATUR_alg() { clear(); }
};

#endif