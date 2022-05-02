#ifndef VERTEX_VISUAL
#define VERTEX_VISUAL

#include "hash_matrix.h"

template <std::size_t size_sq, typename _Alloc = std::allocator<std::list<point_vertex>>>
class vertex_visual
{
protected:
    std::vector<GLfloat> __vec_vertex;
    std::vector<GLfloat> __vec_color;
    std::vector<GLuint> __vec_index_vertex;

    std::tuple<GLfloat, GLfloat, GLfloat> standart_clr;

    hash_matrix<size_sq, _Alloc> mx;

    std::size_t ind_vertex;

    std::size_t __get_color_value(const GLuint &ind) const { return ind * 48; }

    std::size_t __get_vertex_value(const GLuint &ind) const { return ind * 24; }

    std::size_t __get_index_value(const GLuint &ind) const { return ind * 12; }

    void __set_transparency(const std::size_t &ind_color, const GLfloat &alpha_chanel)
    {
        std::cout << "transparency\n";
        std::size_t i = __get_color_value(ind_color);
        std::size_t len = i + 48;

        for (; i < len; i += 4)
        {
            __vec_color[i + 3] = alpha_chanel;
        }
    }

    void __set_point_color(const GLfloat &R, const GLfloat &G, const GLfloat &B, const GLfloat &alpha_chanel = 1.0f)
    {
        __vec_color.emplace_back(R);
        __vec_color.emplace_back(G);
        __vec_color.emplace_back(B);
        // alpha_chanel
        __vec_color.emplace_back(alpha_chanel);
    }

    void __popBackVertex()
    {
        for (std::size_t i = 0; i < 24; ++i)
            __vec_vertex.pop_back();
        for (std::size_t i = 0; i < 33; ++i)
            __vec_index_vertex.pop_back();
        for (std::size_t i = 0; i < 48; ++i)
            __vec_color.pop_back();
        --ind_vertex;
    }

    void __erase_vetex(size_t index)
    {
        size_t ind = __get_vertex_value(index);

        __vec_vertex.erase(__vec_vertex.begin() + ind,
                           __vec_vertex.begin() + ind + 24);

        for (std::size_t i = 0; i < 33; ++i)
            __vec_index_vertex.pop_back();

        for (std::size_t i = 0; i < 48; ++i)
            __vec_color.pop_back();

        --ind_vertex;
    }

    void __emplaceVertexInVector(const std::pair<int, int> &p)
    {
        auto cx = p.first, cy = p.second;
        GLfloat x, y;
        float cnt = 10;
        float a = M_PI * 2 / cnt;
        __vec_vertex.emplace_back(cx);
        __vec_vertex.emplace_back(cy);
        size_t cur_index = __get_index_value(ind_vertex);
        size_t indCenter = cur_index;
        __set_point_color(std::get<0>(standart_clr),
                          std::get<1>(standart_clr),
                          std::get<2>(standart_clr));

        for (std::size_t i = 0; i < cnt + 1; ++i)
        {
            x = sin(a * i) * point_vertex::radius;
            y = cos(a * i) * point_vertex::radius;
            __vec_vertex.emplace_back(x + cx);
            __vec_vertex.emplace_back(y + cy);

            __set_point_color(std::get<0>(standart_clr),
                              std::get<1>(standart_clr),
                              std::get<2>(standart_clr));

            __vec_index_vertex.emplace_back(indCenter);
            __vec_index_vertex.emplace_back(cur_index + 1);
            __vec_index_vertex.emplace_back(cur_index + 2);

            ++cur_index;
        }
        __vec_index_vertex.back() = indCenter + 1;
    }

public:
    vertex_visual(size_t height, size_t width) : ind_vertex(0), mx(height, width), standart_clr(std::make_tuple(0.0F, 0.0F, 0.7F)) {}

    void emplace(const std::pair<int, int> &val)
    {
        mx.emplace(val);

        __emplaceVertexInVector(val);
        ++ind_vertex;
    }

    point_vertex *find(const std::pair<int, int> &val)
    {
        point_vertex *ptr = mx.find(val);
        return ptr;
    }

    std::tuple<size_t *, std::pair<int, int>> erase(const std::pair<int, int> &val)
    {
        auto ptr = mx.find_and_erase(val);
        if (std::get<0>(ptr) != nullptr)
        {
            __erase_vetex(*std::get<0>(ptr));
        }
        return ptr;
    }

    void clear()
    {

        __vec_vertex.clear();

        __vec_color.clear();

        __vec_index_vertex.clear();
        mx.clear();

        ind_vertex = 0;
    }

    void change_the_color_of_the_vertex(const GLuint &ind_color, const GLfloat &R, const GLfloat &G,
                                        const GLfloat &B, const GLfloat &alpha_chanel = 1.0f)
    {
        std::size_t i = __get_color_value(ind_color);
        std::size_t len = i + 48;

        for (; i < len; i += 4)
        {
            __vec_color[i] = R;
            __vec_color[i + 1] = G;
            __vec_color[i + 2] = B;
            __vec_color[i + 3] = alpha_chanel;
        }
    }

    size_t count_of_vertex() const noexcept { return ind_vertex; }

    ~vertex_visual()
    {
        mx.clear();
        clear();
        std::cout << "~vertexVisual\n";
    }

    void print()
    {
        mx.print();
    }

    void print_li()
    {

        mx.print_li();
    }

    GLuint size_vec_color() const noexcept { return __vec_color.size(); }
    GLuint size_vec_vertex() const noexcept { return __vec_vertex.size(); }
    GLuint size_vec_index() const noexcept { return __vec_index_vertex.size(); }

    GLfloat *data_color() { return __vec_color.data(); }
    GLfloat *data_vertex() { return __vec_vertex.data(); }
    GLuint *data_index() { return __vec_index_vertex.data(); }
};

#endif
