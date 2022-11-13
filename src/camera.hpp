#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Camera {
    glm::vec3 eye, lookat, up, right, worldup;
    float yaw, pitch;
    glm::mat4 viewMatrix; 

    Camera() {
        this->eye = glm::vec3(0.f, 0.f, 0.f);
        this->lookat = glm::vec3(0, 0, -1);
        this->up = glm::vec3(0.f, 1.f, 0.f);
        this->yaw = M_PI/2;
        this->pitch = 0;
        this->worldup = this->up;
        updateCameraVectors();
    }
    
    Camera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up, float yaw, float pitch) {
        this->eye = eye;
        this->lookat = lookat;
        this->up = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->worldup = up;
    }
    
    void updateCameraVectors() {
        glm::vec3 front, right;
        front.x = cos(this->yaw) * cos(this->pitch);
        front.y = sin(this->pitch);
        front.z = sin(this->yaw) * cos(this->pitch);
        front = glm::normalize(front);
        right = glm::normalize(glm::cross(front, this->worldup));
        this->up = glm::normalize(glm::cross(right, front));
        this->lookat = this->eye + front;
        this->viewMatrix = glm::lookAt(this->eye, this->lookat, this->up);
    }
};
#endif
