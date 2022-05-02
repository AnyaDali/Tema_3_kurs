#ifndef INTERFACE_GRAPH
#define INTERFACE_GRPAH

#include "brown_alg.h"
#include "DSATUR_alg.h"
#include "graph_visual.h"
#include "color_vertex.h"

template <std::size_t size_sq, typename _Alloc = std::allocator<std::list<point_vertex>>>
class interface_graph
{
protected:
    graph_model __gm;
    graph_visual<size_sq, _Alloc> __gv;

    bool is_painted;

    void __repay_the_vertex_if()
    {
        if (is_painted)
        {
            __gv.repay_vertex();
        }
        is_painted = false;
    }

public:
    interface_graph(std::size_t height, std::size_t width) : __gv(height, width), is_painted(false) {}

    void add_vertex(const std::pair<int, int> &point)
    {
        __repay_the_vertex_if();

        __gv.draw_vertex(point);
        __gm.create_vertex();
    }

    void add_line()
    {
        __repay_the_vertex_if();

        auto __ref_li = __gv.get_li_aiming();
        if (__ref_li.size() == 2)
        {
            __gv.draw_line();
            __gm.add((*__ref_li.cbegin())->indexArray, (*(++__ref_li.cbegin()))->indexArray);
        }
    }

    void erase(const std::pair<int, int> &point)
    {
        __repay_the_vertex_if();

        auto ptr_v = __gv.delete_for_clck(point);
        if (std::get<0>(ptr_v) == graph_visual<size_sq, _Alloc>::lex_delete::DEL_LINE)
        {
            __gm.erase_way(std::get<2>(ptr_v), std::get<3>(ptr_v));
        }
        else if (std::get<0>(ptr_v) == graph_visual<size_sq, _Alloc>::lex_delete::DEL_VERTEX)
        {
            __gm.erase_vertex(std::get<1>(ptr_v));
        }
    }

    void repay_the_vertex() { __gv.repay_the_vertex(); }

    void Dsatur_alg()
    {
        __gv.repay_the_vertex();

        DSATUR_alg obj(__gm);

        obj.DSATUR_algorithm();

        color_vertex cl;
        auto vec_clr = cl.get_vec_color(obj.get_res());
        __gv.color_vertex(vec_clr);
        is_painted = true;
    }

    void Brown_alg()
    {
        __gv.repay_the_vertex();

        algorithm_brown obj(__gm);

        obj.algorithm_brown_exact();

        color_vertex cl;
        auto vec_clr = cl.get_vec_color(obj.get_res());
        __gv.color_vertex(vec_clr);
        is_painted = true;
    }

    void find_vertex(const std::pair<int, int> &point)
    {
        __repay_the_vertex_if();
        __gv.find_vertex(point);
    }

    GLfloat *lines_data() { return __gv.lines_data(); }
    GLuint *lines_index_data() { return __gv.lines_index_data(); }
    GLfloat *lines_color_data() { return __gv.lines_color_data(); }

    size_t lines_size() const noexcept { return __gv.lines_size(); }
    size_t lines_index_size() const noexcept { return __gv.lines_index_size(); }
    size_t lines_color_size() const noexcept { return __gv.lines_color_size(); }

    GLuint vertex_size() const noexcept { return __gv.vertex_size(); }
    GLuint vertex_color_size() const noexcept { return __gv.vertex_color_size(); }
    GLuint vertex_index_size() const noexcept { return __gv.vertex_index_size(); }

    GLfloat *vertex_data() { return __gv.vertex_data(); }
    GLfloat *vertex_color_data() { return __gv.vertex_color_data(); }
    GLuint *vertex_index_data() { return __gv.vertex_index_data(); }

    void clear()
    {
        is_painted = false;
        __gm.clear();
        __gv.clear();
    }

    ~interface_graph()
    {
        clear();
        std::cout << "interface_graph\n";
    }
};

#endif
