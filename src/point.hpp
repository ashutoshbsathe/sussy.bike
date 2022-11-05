#ifndef __POINT_H__
#define __POINT_H__
#include <string>
#include <sstream>
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
struct Point {
    float x, y, z;
    Point();
    Point(float x, float y, float z);
    bool operator<(const struct Point &p) const;
    bool operator==(const struct Point &p) const;
    Point operator=(const struct Point &p);
    Point operator+(const struct Point &p);
    Point operator-(const struct Point &p);
    Point operator*(const glm::mat4 &m);
    Point operator*(const float s);
    std::string to_str();
    glm::vec3 to_vec3();
    glm::vec3 normal;
};
typedef struct Point Point;
#endif
