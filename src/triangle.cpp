#include "includes/triangle.hpp"
#include <sstream>

Triangle::Triangle(Point p1, Point p2, Point p3, Point c) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->c = c;

    glm::vec3 e1 = (p2 - p1).to_vec3(), e2 = (p3 - p1).to_vec3();
    this->normal = glm::normalize(glm::cross(e1, e2));

    std::stringstream ss;
    ss << "[ ";
    ss << "(" << (this->p1).x << ", " << (this->p1).y << ", " << (this->p1).z << ") ";
    ss << "(" << (this->p2).x << ", " << (this->p2).y << ", " << (this->p2).z << ") ";
    ss << "(" << (this->p3).x << ", " << (this->p3).y << ", " << (this->p3).z << ") ";
    ss << "{" << (this->normal).x << ", " << (this->normal).y << ", " << (this->normal).z << "} ";
    ss << "]";
    this->tri_str = ss.str();
}
Triangle::Triangle(Point p1, Point p2, Point p3) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->c = Point(0, 0, 0);

    glm::vec3 e1 = (p2 - p1).to_vec3(), e2 = (p3 - p1).to_vec3();
    this->normal = glm::normalize(glm::cross(e1, e2));

    std::stringstream ss;
    ss << "[ ";
    ss << "(" << (this->p1).x << ", " << (this->p1).y << ", " << (this->p1).z << ") ";
    ss << "(" << (this->p2).x << ", " << (this->p2).y << ", " << (this->p2).z << ") ";
    ss << "(" << (this->p3).x << ", " << (this->p3).y << ", " << (this->p3).z << ") ";
    ss << "{" << (this->normal).x << ", " << (this->normal).y << ", " << (this->normal).z << "} ";
    ss << "]";
    this->tri_str = ss.str();
}
bool Triangle::operator<(const struct Triangle &t) const {
    if(this->p1 == t.p1) {
        if(this->p2 == t.p2) {
            return this->p3 < t.p3;
        } else {
            return this->p2 < t.p2;
        }
    } else {
        return this->p1 < t.p1;
    }
}
bool Triangle::operator==(const struct Triangle &t) const {
    return this->p1 == t.p1 && this->p2 == t.p2 && this->p3 == t.p3;
}
void Triangle::update_str() {
    std::stringstream ss;
    ss << "[ ";
    ss << "(" << (this->p1).x << ", " << (this->p1).y << ", " << (this->p1).z << ") ";
    ss << "(" << (this->p2).x << ", " << (this->p2).y << ", " << (this->p2).z << ") ";
    ss << "(" << (this->p3).x << ", " << (this->p3).y << ", " << (this->p3).z << ") ";
    ss << "{" << (this->normal).x << ", " << (this->normal).y << ", " << (this->normal).z << "} ";
    ss << "]";
    this->tri_str = ss.str();
}
Triangle Triangle::withColor(Point color) {
    return Triangle(this->p1, this->p2, this->p3, color);
}
