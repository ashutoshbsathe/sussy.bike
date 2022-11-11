#ifndef __TEX_H__
#define __TEX_H__

#define STB_IMAGE_IMPLEMENTATION // recommended by `stb_image.h`

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>

#include "stb_image.h"
void loadCubemap(std::vector<std::string> fnames, GLuint *ret) {
    /*
     * fnames must be in
     * right, left, top, bottom, front, back order
     */
    glGenTextures(1, ret);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *ret);

    int width, height, n_channels;
    for(unsigned int i = 0; i < 6; i++) {
        unsigned char *data = stbi_load(fnames[i].c_str(), &width, &height, &n_channels, 0);
        if(data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB,
                GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
            std::cout << "Loaded " << fnames[i] << "\n";
        }
        else {
            std::cout << "Cubemap texture failed to load at: " << fnames[i] << "\n";
            stbi_image_free(data);
            exit(0);
        }
    }
    // TODO: Use MipMaps
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
}
#endif
