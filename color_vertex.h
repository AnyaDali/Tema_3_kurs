#ifndef COLOR_VERTEX_H
#define COLOR_VERTEX_H

#include "graph_model.h"
#include "DSATUR_alg.h"
#include "brown_alg.h"

class color_vertex
{
protected:
    struct clr
    {
        GLfloat R, G, B;
    };

public:
    color_vertex() {}

    std::vector<GLfloat> get_vec_color(const std::pair<std::vector<int>, size_t> &res)
    {
        auto &vec_solution = res.first;
        auto &min_number = res.second;
        std::vector<clr> color(min_number, clr{0.0f, 0.0f, 0.0f});
        if (min_number == 3)
        {
            color[0] = {1.0f, 0.0f, 0.0f};
            color[1] = {0.0f, 1.0f, 0.0f};
            color[2] = {0.0f, 0.0f, 1.0f};
        }
        else if (min_number == 4)
        {
            color[0] = {1.0f, 0.0f, 0.0f};
            color[1] = {0.0f, 1.0f, 0.0f};
            color[2] = {0.0f, 0.0f, 1.0f};
            color[3] = {1.0f, 1.0f, 1.0f};
        }
        else if (min_number == 5)
        {
            color[0] = {1.0f, 0.0f, 0.0f};
            color[1] = {0.0f, 1.0f, 0.0f};
            color[2] = {0.0f, 0.0f, 1.0f};
            color[3] = {1.0f, 1.0f, 1.0f};
            color[4] = {0.0f, 0.0f, 0.0f};
        }
        else
        {
            GLfloat k = 3.0f / static_cast<GLfloat>(min_number);
            GLfloat c = 0.0f;
            clr p{0.0f, 0.0f, 0.0f};
            for (auto &el : color)
            {
                c = k;
                if (p.R < 0.99f)
                {
                    p.R += c;
                    c = (p.R >= 0.99f) ? p.R - 0.99f : 0.0f;
                }

                if (p.G < 0.99f)
                {
                    p.G += c;
                    c = (p.G >= 0.99f) ? p.G - 0.99f : 0.0f;
                }

                if (p.B < 0.99f)
                {
                    p.B += c;
                }

                el = p;
            }
        }

        std::vector<GLfloat> vec_res(vec_solution.size() * 3);

        for (size_t i = 0; i < vec_res.size(); i += 3)
        {
            clr el = color[vec_solution[i / 3]];
            vec_res[i] = el.R;
            vec_res[i + 1] = el.G;
            vec_res[i + 2] = el.B;
        }

        return vec_res;
    }

    ~color_vertex() {}

    friend std::ostream &operator<<(std::ostream &os, color_vertex &obj);
};

#endif