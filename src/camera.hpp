#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Camera {
    glm::vec3 eye, lookat, worldup;
    glm::vec3 n, u, v; // camera vectors
    float yaw, pitch;
    glm::mat4 viewMatrix; 

    Camera() {
        this->eye = glm::vec3(0.f, 0.f, 0.f);
        this->lookat = glm::vec3(0, 0, -1);
        this->worldup = glm::vec3(0.f, 1.f, 0.f);
        this->yaw = 0;
        this->pitch = 0;
        updateCameraVectors();
    }
    
    Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 worldup, float yaw, float pitch) {
        this->eye = eye;
        this->lookat = lookat;
        this->worldup = worldup;
        this->yaw = yaw;
        this->pitch = pitch;
    }
    
    void updateCameraVectors() {
        glm::vec3 new_n;
        new_n.x = cos(this->yaw) * cos(this->pitch);
        new_n.y = sin(this->pitch);
        new_n.z = sin(this->yaw) * cos(this->pitch);
        this->n = glm::normalize(new_n);
        this->u = glm::normalize(glm::cross(n, this->worldup));
        this->v = glm::normalize(glm::cross(this->u, this->n));
        this->viewMatrix = glm::lookAt(this->eye, this->eye + this->n, this->v);
    }
};
#endif
