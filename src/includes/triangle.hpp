#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include "point.hpp"
#include "glm/geometric.hpp"
#include <string>
#include <sstream>
#include <unordered_set>
struct Triangle {
    std::string tri_str;
    Point p1, p2, p3;
    Point c;
    Triangle(Point p1, Point p2, Point p3, Point c);
    Triangle(Point p1, Point p2, Point p3);
    struct Triangle withColor(Point color);
    bool operator<(const struct Triangle &t) const;
    bool operator==(const struct Triangle &t) const;
    void update_str();
    glm::vec3 normal;
    struct HashTriangle {
        size_t operator()(const Triangle &t) const { 
            std::stringstream ss;
            ss << "[ ";
            ss << "(" << (t.p1).x << ", " << (t.p1).y << ", " << (t.p1).z << ") ";
            ss << "(" << (t.p2).x << ", " << (t.p2).y << ", " << (t.p2).z << ") ";
            ss << "(" << (t.p3).x << ", " << (t.p3).y << ", " << (t.p3).z << ") ";
            ss << "{" << (t.normal).x << ", " << (t.normal).y << ", " << (t.normal).z << "} ";
            ss << "]";
            return std::hash<std::string>()(ss.str());
        }
    };
};
typedef struct Triangle Triangle;
#endif
