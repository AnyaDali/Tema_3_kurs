#ifndef GRAPH_VISUAL
#define GRAPH_VISUAL

#include "vertex_visual.h"
#include "line_visual.h"
#include "hash_matrix.h"

template <std::size_t size_sq, typename _Alloc = std::allocator<std::list<point_vertex>>>
class graph_visual
{
protected:
    line_visual __lv;
    vertex_visual<size_sq, _Alloc> __vv;
    std::list<point_vertex *> li_aiming;

public:
    const std::list<point_vertex *> &get_li_aiming() const noexcept { return li_aiming; }

    graph_visual(std::size_t height, std::size_t width) : __vv(height, width) {}

    void draw_vertex(const std::pair<int, int> &point)
    {
        __vv.emplace(point);
    }

    void draw_line()
    {
        if (li_aiming.size() == 2)
        {
            __lv.set_the_line((*li_aiming.begin())->point, (*(++li_aiming.begin()))->point);
            repay_the_vertex();
        }
    }

    point_vertex *find_vertex(const std::pair<int, int> &point)
    {
        point_vertex *ptr = __vv.find(point);

        if (ptr != nullptr)
        {
            if (li_aiming.size() > 0)
            {
                if (((*li_aiming.begin())->point.first == ptr->point.first) &&
                    ((*li_aiming.begin())->point.second == ptr->point.second))
                    return nullptr;
            }

            li_aiming.push_back(ptr);

            __vv.change_the_color_of_the_vertex(li_aiming.back()->indexArray, 0.5f, 0.0f, 1.0f);

            if (li_aiming.size() > 2)
            {
                __vv.change_the_color_of_the_vertex(li_aiming.front()->indexArray, 0.0f, 0.0f, 0.7f);
                li_aiming.pop_front();
            }
        }
        else
        {
            repay_the_vertex();
        }

        return ptr;
    }

    void repay_the_vertex()
    {
        while (!li_aiming.empty())
        {
            __vv.change_the_color_of_the_vertex(li_aiming.back()->indexArray, 0.0f, 0.0f, 0.7f);
            li_aiming.pop_back();
        }
    }

    enum lex_delete
    {
        UNDEFINED,
        DEL_LINE,
        DEL_VERTEX
    };

    std::tuple<lex_delete, int, int, int> delete_for_clck(const std::pair<int, int> &point)
    {
        auto __ptr_v = __vv.erase(point);
        if (std::get<0>(__ptr_v))
        {
            int tmp = static_cast<int>(*std::get<0>(__ptr_v));
            delete std::get<0>(__ptr_v);

            __lv.clear_lines_coord(std::get<1>(__ptr_v));
            return std::make_tuple(DEL_VERTEX, tmp, 0, 0);
        }
        else
        {
            delete std::get<0>(__ptr_v);

            auto __ptr_l = __lv.delete_line(point);

            if (std::get<0>(__ptr_l) == DEL_LINE)
            {

                auto v1 = __vv.find(std::get<1>(__ptr_l));
                auto v2 = __vv.find(std::get<2>(__ptr_l));

                return std::make_tuple(DEL_LINE, 0,
                                       static_cast<int>((*v1).indexArray),
                                       static_cast<int>((*v2).indexArray));
            }
        }

        return std::make_tuple(UNDEFINED, 0, 0, 0);
    }

    void color_vertex(const std::vector<GLfloat> &vec_clr)
    {
        for (size_t i = 0; i < vec_clr.size(); i += 3)
        {
            __vv.change_the_color_of_the_vertex(i / 3, vec_clr[i], vec_clr[i + 1], vec_clr[i + 2]);
        }
    }

    void repay_vertex()
    {
        for (size_t i = 0; i < __vv.count_of_vertex(); ++i)
        {
            __vv.change_the_color_of_the_vertex(i, 0.0f, 0.0f, 0.7f);
        }
    }

    void clear()
    {
        __lv.clear();
        __vv.clear();
        li_aiming.clear();
    }

    ~graph_visual()
    {
        clear();
        std::cout << "~Graph\n";
    }

    GLfloat *lines_data() { return __lv.vec_lines_data(); }
    GLuint *lines_index_data() { return __lv.vec_index_data(); }
    GLfloat *lines_color_data() { return __lv.vec_color_data(); }

    size_t lines_size() const noexcept { return __lv.vec_lines_size(); }
    size_t lines_index_size() const noexcept { return __lv.vec_index_size(); }
    size_t lines_color_size() const noexcept { return __lv.vec_color_size(); }

    GLuint vertex_size() const noexcept { return __vv.size_vec_vertex(); }
    GLuint vertex_color_size() const noexcept { return __vv.size_vec_color(); }
    GLuint vertex_index_size() const noexcept { return __vv.size_vec_index(); }

    GLfloat *vertex_data() { return __vv.data_vertex(); }
    GLfloat *vertex_color_data() { return __vv.data_color(); }
    GLuint *vertex_index_data() { return __vv.data_index(); }
};

#endif