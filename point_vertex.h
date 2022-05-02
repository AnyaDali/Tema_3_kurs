#ifndef POINT_VERTEX
#define POINT_VERTEX

#include "file_include.h"

struct point_vertex
{
public:
    size_t indexArray;
    static std::size_t index;

    std::pair<int, int> point;

    static const GLfloat radius;

    point_vertex(const std::pair<int, int> &point) : point(point), indexArray(index)
    {
        point_vertex::index += 1;
    }

    point_vertex(const std::initializer_list<int> &li_point) : indexArray(index)
    {
        if (li_point.size() != 2)
            throw "error";
        auto ptr = li_point.begin();
        point.first = *ptr;
        point.second = *(++ptr);
        point_vertex::index += 1;
    }

    ~point_vertex()
    {
    }

    bool operator==(const point_vertex &obj) const
    {
        float deltaX = std::fabs(point.first - obj.point.first);
        float deltaY = std::fabs(point.second - obj.point.second);
        float len = std::sqrt(deltaX * deltaX + deltaY * deltaY);

        if (len < radius)
            return true;
        else
            return false;
    }

    bool operator!=(const point_vertex &obj) const { return !this->operator==(obj); }

    friend std::ostream &operator<<(std::ostream &os, const point_vertex &obj);
};

std::ostream &operator<<(std::ostream &os, const point_vertex &obj)
{
    os << '(' << " ind = " << obj.indexArray << ',' << " x = " << obj.point.first << ',' << " y = " << obj.point.second << ')';
    return os;
}


const float point_vertex::radius = 10.0f;
std::size_t point_vertex::index = 0;

#endif
