#ifndef LINE_VISUAL
#define LINE_VISUAL

#include "point_vertex.h"

class line_visual
{
protected:
    std::vector<GLfloat> vec_lines; // массив вершин для отрисовки рёбер графа
    std::vector<GLuint> vec_index;  // массив индексов для отрисовки рёбер графа
    std::vector<GLfloat> vec_color; // массив цветов для отрисовки рёбер
    std::size_t ind_line;

    std::size_t __get_color_value(const std::size_t &ind) const noexcept
    {
        return ind * 6;
    }

    std::size_t __get_line_value(const std::size_t &ind) const noexcept
    {
        return ind * 4;
    }

    std::size_t __get_index_value(const std::size_t &ind) const noexcept
    {
        return ind * 2;
    }

    void __set_color(const GLfloat &R, const GLfloat &G, const GLfloat &B)
    {
        vec_color.push_back(R);
        vec_color.push_back(G);
        vec_color.push_back(B);

        vec_color.push_back(R);
        vec_color.push_back(G);
        vec_color.push_back(B);
    }

    void __set_index()
    {
        size_t i = __get_index_value(ind_line);
        vec_index.push_back(i);
        vec_index.push_back(i + 1);
    }

    void __set_line(const std::pair<int, int> &p1, const std::pair<int, int> &p2)
    {
        vec_lines.push_back(p1.first);
        vec_lines.push_back(p1.second);
        vec_lines.push_back(p2.first);
        vec_lines.push_back(p2.second);
    }

    void __delete_line(const size_t ind)
    {
        size_t i = __get_line_value(ind);
        vec_lines.erase(vec_lines.begin() + i, vec_lines.begin() + i + 4);
    }

    void __delete_index()
    {
        vec_index.pop_back();
        vec_index.pop_back();
    }

    void __delete_color()
    {
        for (size_t i = 0; i < 6; ++i)
        {
            vec_color.pop_back();
        }
    }

    void __delete_line_obj(const size_t &ind)
    {
        __delete_line(ind);
        __delete_color();
        __delete_index();
    }

public:
    line_visual() : ind_line(0) {}

    void change_color_of_the_line(const std::size_t &ind,
                                  const GLfloat &R, const GLfloat &G, const GLfloat &B)
    {
        std::size_t i = __get_color_value(ind);
        vec_color[i] = R;
        vec_color[i + 1] = G;
        vec_color[i + 2] = B;
        vec_color[i + 3] = R;
        vec_color[i + 4] = G;
        vec_color[i + 5] = B;
    }

    void set_the_line(const std::pair<int, int> &p1, const std::pair<int, int> &p2)
    {
        __set_color(0.0F, 0.0F, 0.0F);
        __set_index();
        __set_line(p1, p2);
        ++ind_line;
    }

    std::tuple<size_t *, std::pair<int, int>, std::pair<int, int>> find(const std::pair<int, int> &point)
    {
        for (std::size_t i = 0; i < vec_lines.size(); i += 4)
        {
            glm::vec2 a = {vec_lines[i], vec_lines[i + 1]};
            glm::vec2 b = {vec_lines[i + 2], vec_lines[i + 3]};
            glm::vec2 p = {point.first, point.second};
            if (check_line(a, b, p))
            {
                size_t *ind = new size_t(i / 4);
                return std::make_tuple(new size_t(i / 4),
                                       std::pair<int, int>{vec_lines[i], vec_lines[i + 1]},
                                       std::pair<int, int>{vec_lines[i + 2], vec_lines[i + 3]});
            }
        }

        return std::make_tuple(nullptr,
                               std::pair<int, int>{0, 0},
                               std::pair<int, int>{0, 0});
    }

    size_t *find(const std::pair<int, int> &point1, const std::pair<int, int> &point2)
    {
        size_t *ind = nullptr;
        for (std::size_t i = 0; i < vec_lines.size(); i += 4)
        {
            if ((vec_lines[i] == point1.first && vec_lines[i + 1] == point1.second &&
                 vec_lines[i + 2] == point2.first && vec_lines[i + 3] == point2.second) ||
                (vec_lines[i] == point2.first && vec_lines[i + 1] == point2.second &&
                 vec_lines[i + 2] == point1.first && vec_lines[i + 3] == point1.second))
            {
                ind = new size_t(i / 4);
                break;
            }
        }
        return ind;
    }

    void delete_line(const std::pair<int, int> &point1, const std::pair<int, int> &point2)
    {
        size_t *i = find(point1, point2);
        if (i != nullptr)
        {
            __delete_line_obj(*i);
            delete i;
            --ind_line;
        }
    }

    std::tuple<int, std::pair<int, int>, std::pair<int, int>>
    delete_line(const std::pair<int, int> &point)
    {
        auto i = find(point);
        if (std::get<0>(i) != nullptr)
        {
            __delete_line_obj(*std::get<0>(i));
            delete std::get<0>(i);
            --ind_line;
            return std::make_tuple(1, std::get<1>(i), std::get<2>(i));
        }
        return std::make_tuple(0,
                               std::pair<int, int>{0, 0},
                               std::pair<int, int>{0, 0});
    }

    bool check_line(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &p)
    {
        const glm::vec2 a2b = b - a;
        const glm::vec2 a2p = p - a;
        double area = a2b.x * a2p.y - a2p.x * a2b.y;

        if ((a2b.x * a2p.x < 0) || (a2b.y * a2p.y < 0))
        {
            return false;
        }
        if (glm::length(a2b) < glm::length(a2p))
        {
            return false;
        }
        if (glm::length(p - a) < 10.0f)
        {
            return false;
        }
        if (glm::length(p - b) < 10.0f)
        {
            return false;
        }
        if (glm::abs(area) < 400.0f)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void clear_lines_coord(const std::pair<int, int> &point)
    {
        for (int i = 0; i < vec_lines.size(); i += 2)
        {
            if (point.first == vec_lines[i] && point.second == vec_lines[i + 1])
            {
                __delete_line_obj(i / 4);
                --ind_line;
                i = -2;
            }
        }
    }

    void clear()
    {
        vec_lines.clear();
        vec_index.clear();
        vec_color.clear();
        ind_line = 0;
    }

    ~line_visual()
    {
        clear();
    }

    GLfloat *vec_lines_data() { return vec_lines.data(); }
    GLuint *vec_index_data() { return vec_index.data(); }
    GLfloat *vec_color_data() { return vec_color.data(); }

    size_t vec_lines_size() const noexcept { return vec_lines.size(); }
    size_t vec_index_size() const noexcept { return vec_index.size(); }
    size_t vec_color_size() const noexcept { return vec_color.size(); }
};

#endif