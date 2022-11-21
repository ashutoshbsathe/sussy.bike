#ifndef _SPPRISM_HPP_
#define _SPPRISM_HPP_
#include <vector>
#include <cmath>
#include <cassert>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include "point.hpp"
#include "triangle.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

struct StackedPolyPrism {
    /* A stacked polygon prism 
     * with `n` levels of `k` sided polygons
     * All magic is done in the constructor
     * Renderer should just access `triangle_list`
     */
    std::string name;
    unsigned int n, k;
    bool top_closed = false, bot_closed = false;
    /* Length of these vectors is `n` */
    std::vector<float> rs, hs;
    std::vector<Point> colors;
    glm::mat4 init_transform;

    std::vector<Point> polyAt(float h, float r, unsigned int k);
    std::vector<Triangle> trapezoidsBetweenLevels(std::vector<Point> l1, std::vector<Point> l2, Point c);

    std::vector<std::vector<Point>> points;
    std::vector<Triangle> triangle_list;
    void build_triangle_list(bool transform);
    StackedPolyPrism(unsigned int n, unsigned int k, std::vector<float> rs, std::vector<float> hs, bool top_closed, bool bot_closed, std::vector<Point> colors, glm::mat4 init_transform);
    StackedPolyPrism(std::string fname);

    std::string to_str();
};
#endif
