#ifndef GRAPH_ALG_H
#define GRAPH_ALG_H

#include "graph_visual.h"

template <std::size_t size_sq, typename _Alloc = std::allocator<std::list<point_vertex>>>
class graph_algorithm : public graph_visual<size_sq, _Alloc>
{
protected:
    std::vector<int> colorArr;

    struct node_info
    {
        int sat;        // степень насыщенности вершины
        int deg;        // степень вершины
        int index_vert; // индекс вершины
    };

    struct comparator_info
    {
        bool operator()(const node_info &lhs,
                        const node_info &rhs) const
        {
            return std::tie(lhs.sat, lhs.deg, lhs.index_vert) > std::tie(rhs.sat, rhs.deg, rhs.index_vert);
        }
    };

public:
    graph_algorithm(std::size_t height, std::size_t width) : graph_visual<size_sq, _Alloc>::graph_visual(height, width) {}

    void DSATUR_algorithm()
    {
        std::size_t n = graph_visual<size_sq, _Alloc>::bool_mx.size_n();

        colorArr.resize(n);

        int u, i;
        std::vector<bool> used(n, false);
        std::vector<int> d(n); // d(n) - кол-во смежных вершин
        std::vector<std::set<int>> adjCols(n, std::set<int>());
        std::set<graph_algorithm<size_sq, _Alloc>::node_info, graph_algorithm<size_sq, _Alloc>::comparator_info> Q;
        auto maxPtr = Q.begin();
        std::vector<std::vector<int>> adj(n);
        for (u = 0; u < n; u++)
        {
            d[u] = graph_visual<size_sq, _Alloc>::bool_mx.calc_degress(u, adj);
            std::cout << "size = " << d[u] << std::endl;
            Q.emplace(node_info{0, d[u], u}); // заполения дерева
        }

        while (!Q.empty())
        {
            // Choose the vertex u
            // with highest saturation
            // degree, breaking ties with d.
            // Remove u from the priority queue
            maxPtr = Q.begin();
            u = (*maxPtr).index_vert;
            std::cout << u << std::endl;
            Q.erase(maxPtr);

            // Identify the lowest feasible
            // colour i for vertex u
            for (int v : adj[u])
                if (colorArr[v] != -1)
                    used[colorArr[v]] = true;

            for (i = 0; i < used.size(); i++)
                if (used[i] == false)
                    break;

            for (int v : adj[u])
                if (colorArr[v] != -1)
                    used[colorArr[v]] = false;

            // Assign vertex u to colour i
            colorArr[u] = i;

            // Update the saturation degrees and
            // degrees of all uncoloured neighbours;
            // hence modify their corresponding
            // elements in the priority queue
            for (int v : adj[u])
            {
                if (colorArr[v] == -1)
                {
                    Q.erase(
                        {int(adjCols[v].size()),
                         d[v], v});
                    adjCols[v].insert(i);
                    d[v]--;
                    Q.emplace(node_info{
                        int(adjCols[v].size()),
                        d[v], v});
                }
            }
        }

        // The full graph has been coloured.
        // Print the result
        for (u = 0; u < n; u++)
            std::cout << "Vertex " << u
                      << " --->  Color " << colorArr[u]
                      << std::endl;
        color_the_graph();
    }

    void color_the_graph()
    {
        GLfloat R, G, B;
        for (std::size_t i = 0; i < colorArr.size(); ++i)
        {
            R = G = B = 1.0f;
            if (colorArr[i] == 0)
                R = 0.1f;
            else if (colorArr[i] == 1)
                G = 0.1f;
            else if (colorArr[i] == 2)
                B = 0.1f;
            else if (colorArr[i] == 3)
            {
                R = 0.5f;
                G = 0.5f;
            }
            vertex_visual<size_sq, _Alloc>::change_the_color_of_the_vertex(i, R, G, B);
        }
    }

    ~graph_algorithm() {}
};

#endif