#ifndef __LIGHTS_H__
#define __LIGHTS_H__
#include "camera.hpp"
#include "glm/vec3.hpp"
struct Light {
    glm::vec3 position, spotPoint;
    float cutOff;
    bool isActive;
    Light(glm::vec3 position, glm::vec3 spotPoint, float cutOff, bool isActive) {
        this->position = position;
        this->spotPoint = spotPoint;
        this->cutOff = cutOff;
        this->isActive = isActive;
    }
    Camera to_camera() {
        return Camera(this->position, this->spotPoint, glm::vec3(0.f, 1.f, 0.f));
    }
};
#endif
