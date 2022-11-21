#include "includes/point.hpp"

Point::Point() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}
Point::Point(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
bool Point::operator<(const struct Point &p) const {
    if(this->x == p.x) {
        if(this->y == p.y) {
            return this->z < p.z;
        } else {
            return this->y < p.y;
        }
    } else {
        return this->x < p.x;
    }
}
bool Point::operator==(const struct Point &p) const {
    return this->x == p.x && this->y == p.y && this->z == p.z;
}
Point Point::operator=(const struct Point &p) {
    this->x = p.x;
    this->y = p.y;
    this->z = p.z;
    return *this;
}
Point Point::operator+(const struct Point &p) {
    Point ret;
    ret.x = this->x + p.x;
    ret.y = this->y + p.y;
    ret.z = this->z + p.z;
    return ret;
}
Point Point::operator-(const struct Point &p) {
    Point ret;
    ret.x = this->x - p.x;
    ret.y = this->y - p.y;
    ret.z = this->z - p.z;
    return ret;
}
Point Point::operator*(const glm::mat4 &m) {
    glm::vec4 p(this->x, this->y, this->z, 1);
    p = m * p;
    Point ret(p.x/p.w, p.y/p.w, p.z/p.w);
    return ret;
}
Point Point::operator*(const float s) {
    Point ret(this->x * s, this->y * s, this->z * s);
    return ret;
}
std::string Point::to_str() {
    std::stringstream pt;
    pt << "(" << this->x << ", " << this->y << ", " << this->z << ")";
    pt << ", normal = [" << this->normal.x << ", " << this->normal.y << ", " << this->normal.z << "]";
    return pt.str();
}
glm::vec3 Point::to_vec3() {
    return glm::vec3(this->x, this->y, this->z);
}
