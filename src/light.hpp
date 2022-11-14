#ifndef __LIGHTS_H__
#define __LIGHTS_H__
#include <vector>
#include <sstream>
#include <cmath>
#include <GL/glew.h>
#include "camera.hpp"
#include "glm/vec3.hpp"
struct Light {
    glm::vec3 position, spotPoint;
    float cutOff;
    Light(glm::vec3 position, glm::vec3 spotPoint, float cutOff) {
        this->position = position;
        this->spotPoint = spotPoint;
        this->cutOff = cutOff;
    }
    Camera to_camera() {
        return Camera(this->position, this->spotPoint, glm::vec3(0.f, 1.f, 0.f));
    }
};

std::vector<Light> all_lights = {
    Light(
        glm::vec3(12500.f, 12500.f, 12500.f),
        glm::vec3(0.f, 0.f, 0.f),
        -1.5f
    ),
    Light(
        glm::vec3(-12500.f, 12500.f, -12500.f),
        glm::vec3(0.f, 0.f, 0.f),
        -1.5f
    )
};

void push_lights_to_uniform(GLuint shader_program) {
    unsigned num_lights = all_lights.size();
    GLuint tmp;
    glm::vec3 dir;
    std::stringstream ss;
    for(unsigned int i = 0; i < num_lights; i++) {
        ss.clear();
        ss << "lights[" << i << "].position";
        tmp = glGetUniformLocation(shader_program, ss.str().c_str());
        glUniform3f(tmp, all_lights[i].position.x, all_lights[i].position.y, all_lights[i].position.z);
        dir = -glm::normalize(all_lights[i].to_camera().n);
        ss.clear();
        ss << "lights[" << i << "].spotDir";
        tmp = glGetUniformLocation(shader_program, ss.str().c_str());
        glUniform3f(tmp, dir.x, dir.y, dir.z);
        ss.clear();
        ss << "lights[" << i << "].cutOff";
        tmp = glGetUniformLocation(shader_program, ss.str().c_str());
        glUniform1f(tmp, all_lights[i].cutOff);
    }
}

#endif
