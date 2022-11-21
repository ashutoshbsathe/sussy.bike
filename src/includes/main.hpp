#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include "point.hpp"
#include "triangle.hpp" 
#include "spprism.hpp"
#include "hnode.hpp"
#include "entity.hpp"
#include "tex.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "state.hpp"

#include "gl_framework.hpp"
#include "shader_util.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iomanip>

#define FRAMERATE 60.0

#define TORSO_2(X) ((X)->children[0]) 
#define NECK(X) ((TORSO_2(X))->children[0])
#define HEAD(X) ((NECK(X))->children[0])
#define HEAD_GLOBAL_TRANSFORM(X) (X->local_transform * X->dof_transform * TORSO_2(X)->local_transform * TORSO_2(X)->dof_transform * NECK(X)->local_transform * NECK(X)->dof_transform * HEAD(X)->local_transform * HEAD(X)->dof_transform)

float xrot = 0, yrot = 0, zrot = 0, rotamount = M_PI/30.0, light_x = 0, light_y = 0, light_z = 0, light_moveamount = M_PI/303.0, xmove = 0, ymove = 0, zmove = 0, moveamount = 50;



void push_lights_to_uniform(GLuint shader_program, std::vector<Light> all_lights) {
    unsigned num_lights = all_lights.size();
    GLuint tmp;
    glm::vec3 dir;
    std::stringstream ss;
    for(unsigned int i = 0; i < num_lights; i++) {
        ss.str(std::string());
        ss << "lights[" << i << "].position";
        tmp = glGetUniformLocation(shader_program, ss.str().c_str());
        glUniform3f(tmp, all_lights[i].position.x, all_lights[i].position.y, all_lights[i].position.z);
        dir = -glm::normalize(all_lights[i].to_camera().n);
        ss.str(std::string());
        ss << "lights[" << i << "].spotDir";
        tmp = glGetUniformLocation(shader_program, ss.str().c_str());
        glUniform3f(tmp, dir.x, dir.y, dir.z);
        ss.str(std::string());
        ss << "lights[" << i << "].cutOff";
        tmp = glGetUniformLocation(shader_program, ss.str().c_str());
        glUniform1f(tmp, all_lights[i].cutOff);
        ss.str(std::string());
        ss << "lights[" << i << "].isActive";
        tmp = glGetUniformLocation(shader_program, ss.str().c_str());
        glUniform1i(tmp, all_lights[i].isActive);
    }
}

// Global rendering related things
GLuint shader_program, vbo, vao, uModelMatrix_id, uNormalMatrix_id, uViewMatrix_id, position_id, color_id, normal_id, uLightSpaceMatrix_id, uShadowMap_id, uNumLights_id, uMaterial_id;

glm::mat4 projection_matrix;
glm::mat4 modelviewproject_matrix;
glm::mat4 lightspace_matrix;
glm::mat4 rotation_matrix;
glm::mat4 light_movement_matrix;
glm::mat3 normal_matrix;

// Shadowmaps
GLuint shadow_shader_program, shadow_position_id, shadow_uLightSpaceMatrix_id;

glm::mat4 view_matrix;
GLuint depthMapFBO, depthMap_width = 1024, depthMap_height = 1024, depthMap_texture, depthMap_texture_array;

// bike headlight
glm::vec3 bike_headlight, bike_headlight_lookat_dir;

// third person camera
glm::vec3 third_person_position, third_person_lookat;

// first person camera
glm::vec3 first_person_position, first_person_lookat_dir;

// skybox
std::vector<std::string> skybox_fnames = {
    "./resources/skybox_sand/posx.jpg",
    "./resources/skybox_sand/negx.jpg",
    "./resources/skybox_sand/posy.jpg",
    "./resources/skybox_sand/negy.jpg",
    "./resources/skybox_sand/posz.jpg",
    "./resources/skybox_sand/negz.jpg",
};
GLuint skybox_texture, skybox_vao, skybox_vbo, skybox_shader_program, skybox_position_id, skybox_uModelViewProject_id, skybox_sampler_id;
float skybox_radius = 102400.f;

/* skybox_vertices_points
              (+Y)
              |
              g--------f
             /|       /|
            / |      / |
           h--------e  |
           |  |     |  |
           |  c-----|--b----(+X)
           | /      | /
           |/       |/
           d--------a
          /
         /
      (+Z)
*/
Point c = Point(-skybox_radius, -skybox_radius, -skybox_radius);
Point a = Point(skybox_radius, -skybox_radius, skybox_radius);
Point b = Point(skybox_radius, -skybox_radius, -skybox_radius);
Point d = Point(-skybox_radius, -skybox_radius, skybox_radius);
Point e = Point(skybox_radius, skybox_radius, skybox_radius);
Point f = Point(skybox_radius, skybox_radius, -skybox_radius);
Point g = Point(-skybox_radius, skybox_radius, -skybox_radius);
Point h = Point(-skybox_radius, skybox_radius, skybox_radius);
std::vector<Triangle> skybox_triangle_list = {
    Triangle(c, h, g), Triangle(c, d, h),
    Triangle(b, a, e), Triangle(b, e, f),
    Triangle(c, a, d), Triangle(c, b, a),
    Triangle(g, e, h), Triangle(g, f, e),
    Triangle(d, e, a), Triangle(d, h, e),
    Triangle(c, f, b), Triangle(c, g, f)
};

void initSkyboxShadersGL(void) {
    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, "skybox_vs.glsl"));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, "skybox_fs.glsl"));

    skybox_shader_program = csX75::CreateProgramGL(shaderList);
    skybox_position_id = glGetAttribLocation(skybox_shader_program, "vPosition");
    skybox_uModelViewProject_id = glGetUniformLocation(skybox_shader_program, "uModelViewProjectMatrix");
    skybox_sampler_id = glGetUniformLocation(skybox_shader_program, "skybox");
}

void initSkyboxBuffersGL(void) {
    /* Init for skybox */
    loadCubemap(skybox_fnames, &skybox_texture);
    float skybox_vertices[12 * 3 * 3];
    for(unsigned int i = 0; i < 12; i++) {
        skybox_vertices[9*i] = skybox_triangle_list[i].p1.x;
        skybox_vertices[9*i+1] = skybox_triangle_list[i].p1.y;
        skybox_vertices[9*i+2] = skybox_triangle_list[i].p1.z;
        skybox_vertices[9*i+3] = skybox_triangle_list[i].p2.x;
        skybox_vertices[9*i+4] = skybox_triangle_list[i].p2.y;
        skybox_vertices[9*i+5] = skybox_triangle_list[i].p2.z;
        skybox_vertices[9*i+6] = skybox_triangle_list[i].p3.x;
        skybox_vertices[9*i+7] = skybox_triangle_list[i].p3.y;
        skybox_vertices[9*i+8] = skybox_triangle_list[i].p3.z;
    }
    glGenVertexArrays (1, &skybox_vao);
    glBindVertexArray (skybox_vao);
    glGenBuffers (1, &skybox_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, skybox_vbo);
 
    glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices) , &skybox_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(skybox_position_id);
    glVertexAttribPointer(shadow_position_id, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), BUFFER_OFFSET(0));
}

void renderSkyboxGL(glm::mat4 modelviewproject) {
    glDepthMask(GL_FALSE);
    glUseProgram(skybox_shader_program);
    glUniformMatrix4fv(skybox_uModelViewProject_id, 1, GL_FALSE, glm::value_ptr(modelviewproject));
    glUniform1i(skybox_sampler_id, 0);
    glBindVertexArray(skybox_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

// Texture specific globals
/*
    4----------2   
    |*         |
    |   *      |
    |      *   |
    3---------*1
*/
GLuint texture_vao, texture_vbo, texture_shader_program, texture_position_id, texture_texPosition_id, texture_normal_id, texture_uModelMatrix_id, texture_uNormalMatrix_id, texture_uViewMatrix_id, texture_sampler_id, texture_shadow_id, texture_uNumLights_id, texture_uMaterial_id, texture_uLightSpaceMatrix_id;

void initTexturedShadersGL(void) {
    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, "texture_vs.glsl"));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, "texture_fs.glsl"));

    texture_shader_program = csX75::CreateProgramGL(shaderList);
    texture_position_id = glGetAttribLocation(texture_shader_program, "vPosition");
    texture_texPosition_id = glGetAttribLocation(texture_shader_program, "vTexture");
    texture_normal_id = glGetAttribLocation(texture_shader_program, "vNormal");
    texture_uModelMatrix_id = glGetUniformLocation(texture_shader_program, "uModelMatrix");
    texture_uNormalMatrix_id = glGetUniformLocation(texture_shader_program, "uNormalMatrix");
    texture_uViewMatrix_id = glGetUniformLocation(texture_shader_program, "uViewMatrix");
    texture_uLightSpaceMatrix_id = glGetUniformLocation(texture_shader_program, "uLightSpaceMatrix");
    texture_uNumLights_id = glGetUniformLocation(texture_shader_program, "num_lights");
    texture_uMaterial_id = glGetUniformLocation(texture_shader_program, "material");
    texture_shadow_id = glGetUniformLocation(texture_shader_program, "shadowMap");
    texture_sampler_id = glGetUniformLocation(texture_shader_program, "tex");
}

// Sand track texture
std::string sandTrack_texture_fname = "./resources/skybox_test/track.png";
GLuint sandTrack_texture_id;

// Bike headlight texture
std::string bikeHeadlight_texture_fname = "./resources/skybox_test/bike.png";
GLuint bikeHeadlight_texture_id;

// Humanoid shirt texture
std::string humanoidShirt_texture_fname = "./resources/skybox_test/rider.png";
GLuint humanoidShirt_texture_id;

// Common function for all textured entities
void initTexturedBuffersGL(void) {
    unsigned int j = 0;
    std::vector<Triangle> all_triangle_list;
    std::vector<float> all_tex_vertices;
    float all_vbo_vertices[1024]; // more than what is needed

    /* Init for sandTrack */
    loadTexmap(sandTrack_texture_fname, &sandTrack_texture_id);
    Point p1_sand(1500+5000,-1500,-700);   //bottom right
    Point p2_sand(1500+5000,1500,-700);    //bottom left
    Point p3_sand(-1500+5000,-1500,-700);  //top right
    Point p4_sand(-1500+5000,1500,-700);   //top left

    all_triangle_list.push_back(Triangle(p1_sand,p4_sand,p3_sand));
    all_triangle_list.push_back(Triangle(p4_sand,p1_sand,p2_sand));

    all_tex_vertices.push_back(1.0f); all_tex_vertices.push_back(1.0f); //p1
    all_tex_vertices.push_back(0.0f); all_tex_vertices.push_back(0.0f); //p4
    all_tex_vertices.push_back(1.0f); all_tex_vertices.push_back(0.0f); //p3
    all_tex_vertices.push_back(0.0f); all_tex_vertices.push_back(0.0f); //p4
    all_tex_vertices.push_back(1.0f); all_tex_vertices.push_back(1.0f); //p1
    all_tex_vertices.push_back(0.0f); all_tex_vertices.push_back(1.0f); //p2

    for(unsigned int i = 0; i < all_triangle_list.size(); i++) {
        all_vbo_vertices[24*i] = all_triangle_list[i].p1.x;
        all_vbo_vertices[24*i+1] = all_triangle_list[i].p1.z+500;
        all_vbo_vertices[24*i+2] = all_triangle_list[i].p1.y;
        all_vbo_vertices[24*i+3] = all_triangle_list[i].normal.x;
        all_vbo_vertices[24*i+4] = all_triangle_list[i].normal.y;
        all_vbo_vertices[24*i+5] = all_triangle_list[i].normal.z;
        all_vbo_vertices[24*i+6] = all_tex_vertices[j++];
        all_vbo_vertices[24*i+7] = all_tex_vertices[j++];

        all_vbo_vertices[24*i+8] = all_triangle_list[i].p2.x;
        all_vbo_vertices[24*i+9] = all_triangle_list[i].p2.z+500;
        all_vbo_vertices[24*i+10] = all_triangle_list[i].p2.y;
        all_vbo_vertices[24*i+11] = all_triangle_list[i].normal.x;
        all_vbo_vertices[24*i+12] = all_triangle_list[i].normal.y;
        all_vbo_vertices[24*i+13] = all_triangle_list[i].normal.z;
        all_vbo_vertices[24*i+14] = all_tex_vertices[j++];
        all_vbo_vertices[24*i+15] = all_tex_vertices[j++];

        all_vbo_vertices[24*i+16] = all_triangle_list[i].p3.x;
        all_vbo_vertices[24*i+17] = all_triangle_list[i].p3.z+500;
        all_vbo_vertices[24*i+18] = all_triangle_list[i].p3.y;
        all_vbo_vertices[24*i+19] = all_triangle_list[i].normal.x;
        all_vbo_vertices[24*i+20] = all_triangle_list[i].normal.y;
        all_vbo_vertices[24*i+21] = all_triangle_list[i].normal.z;
        all_vbo_vertices[24*i+22] = all_tex_vertices[j++];
        all_vbo_vertices[24*i+23] = all_tex_vertices[j++];
    }
    /* Init for bikeHeadlight */
    loadTexmap(bikeHeadlight_texture_fname, &bikeHeadlight_texture_id);

    float offset_x=25, offset_y=0, offset_z=0;
    Point p1_bike(750+offset_x, 675+offset_y, 0+offset_z);  //top right
    Point p2_bike(875+offset_x, 487.5+offset_y, 0+offset_z);    //bottom right
    Point p3_bike(875+offset_x, 487.5+offset_y, 150+offset_z);  //bottom left
    Point p4_bike(750+offset_x, 675+offset_y, 150+offset_z);    //top left

    all_triangle_list.push_back(Triangle(p1_bike,p4_bike,p3_bike));
    all_triangle_list.push_back(Triangle(p2_bike,p1_bike,p3_bike));

    all_tex_vertices.push_back(1.0f); all_tex_vertices.push_back(0.0f); // p1
    all_tex_vertices.push_back(0.0f); all_tex_vertices.push_back(0.0f); // p4
    all_tex_vertices.push_back(0.0f); all_tex_vertices.push_back(1.0f); // p3
    all_tex_vertices.push_back(1.0f); all_tex_vertices.push_back(1.0f); // p2
    all_tex_vertices.push_back(1.0f); all_tex_vertices.push_back(0.0f); // p1
    all_tex_vertices.push_back(0.0f); all_tex_vertices.push_back(1.0f); // p3
    for(unsigned int i = 2; i < all_triangle_list.size(); i++) {
        all_vbo_vertices[24*i] = all_triangle_list[i].p1.x;
        all_vbo_vertices[24*i+1] = all_triangle_list[i].p1.y;
        all_vbo_vertices[24*i+2] = all_triangle_list[i].p1.z;
        all_vbo_vertices[24*i+3] = all_triangle_list[i].normal.x;
        all_vbo_vertices[24*i+4] = all_triangle_list[i].normal.y;
        all_vbo_vertices[24*i+5] = all_triangle_list[i].normal.z;
        all_vbo_vertices[24*i+6] = all_tex_vertices[j++];
        all_vbo_vertices[24*i+7] = all_tex_vertices[j++];

        all_vbo_vertices[24*i+8] = all_triangle_list[i].p2.x;
        all_vbo_vertices[24*i+9] = all_triangle_list[i].p2.y;
        all_vbo_vertices[24*i+10] = all_triangle_list[i].p2.z;
        all_vbo_vertices[24*i+11] = all_triangle_list[i].normal.x;
        all_vbo_vertices[24*i+12] = all_triangle_list[i].normal.y;
        all_vbo_vertices[24*i+13] = all_triangle_list[i].normal.z;
        all_vbo_vertices[24*i+14] = all_tex_vertices[j++];
        all_vbo_vertices[24*i+15] = all_tex_vertices[j++];

        all_vbo_vertices[24*i+16] = all_triangle_list[i].p3.x;
        all_vbo_vertices[24*i+17] = all_triangle_list[i].p3.y;
        all_vbo_vertices[24*i+18] = all_triangle_list[i].p3.z;
        all_vbo_vertices[24*i+19] = all_triangle_list[i].normal.x;
        all_vbo_vertices[24*i+20] = all_triangle_list[i].normal.y;
        all_vbo_vertices[24*i+21] = all_triangle_list[i].normal.z;
        all_vbo_vertices[24*i+22] = all_tex_vertices[j++];
        all_vbo_vertices[24*i+23] = all_tex_vertices[j++];
    }
    
    /* Init for humanoidShirt */
    loadTexmap(humanoidShirt_texture_fname, &humanoidShirt_texture_id);
    Point p1(70, 121.244, -130);    //bottom left
    Point p2(-97.5, 168.875, 50);   //top right
    Point p3(97.5, 168.875, 50);    //top left
    Point p4(-70, 121.244, -130);   //bottom right

    all_triangle_list.push_back(Triangle(p4,p2,p3));
    all_triangle_list.push_back(Triangle(p1,p4,p3));
    float z_offset = 180;

    all_tex_vertices.push_back(1.0f); all_tex_vertices.push_back(1.0f); //p4
    all_tex_vertices.push_back(1.0f); all_tex_vertices.push_back(0.0f); //p2
    all_tex_vertices.push_back(0.0f); all_tex_vertices.push_back(0.0f); //p3
    all_tex_vertices.push_back(0.0f); all_tex_vertices.push_back(1.0f); //p1
    all_tex_vertices.push_back(1.0f); all_tex_vertices.push_back(1.0f); //p4
    all_tex_vertices.push_back(0.0f); all_tex_vertices.push_back(0.0f); //p3

    for(unsigned int i = 4; i < all_triangle_list.size(); i++) {
        all_vbo_vertices[24*i] = all_triangle_list[i].p1.x;
        all_vbo_vertices[24*i+1] = all_triangle_list[i].p1.y*0.775;
        all_vbo_vertices[24*i+2] = all_triangle_list[i].p1.z + z_offset;
        all_vbo_vertices[24*i+3] = all_triangle_list[i].normal.x;
        all_vbo_vertices[24*i+4] = all_triangle_list[i].normal.y;
        all_vbo_vertices[24*i+5] = all_triangle_list[i].normal.z;
        all_vbo_vertices[24*i+6] = all_tex_vertices[j++];
        all_vbo_vertices[24*i+7] = all_tex_vertices[j++];

        all_vbo_vertices[24*i+8] = all_triangle_list[i].p2.x;
        all_vbo_vertices[24*i+9] = all_triangle_list[i].p2.y*0.775;
        all_vbo_vertices[24*i+10] = all_triangle_list[i].p2.z + z_offset;
        all_vbo_vertices[24*i+11] = all_triangle_list[i].normal.x;
        all_vbo_vertices[24*i+12] = all_triangle_list[i].normal.y;
        all_vbo_vertices[24*i+13] = all_triangle_list[i].normal.z;
        all_vbo_vertices[24*i+14] = all_tex_vertices[j++];
        all_vbo_vertices[24*i+15] = all_tex_vertices[j++];

        all_vbo_vertices[24*i+16] = all_triangle_list[i].p3.x;
        all_vbo_vertices[24*i+17] = all_triangle_list[i].p3.y*0.775;
        all_vbo_vertices[24*i+18] = all_triangle_list[i].p3.z + z_offset;
        all_vbo_vertices[24*i+19] = all_triangle_list[i].normal.x;
        all_vbo_vertices[24*i+20] = all_triangle_list[i].normal.y;
        all_vbo_vertices[24*i+21] = all_triangle_list[i].normal.z;
        all_vbo_vertices[24*i+22] = all_tex_vertices[j++];
        all_vbo_vertices[24*i+23] = all_tex_vertices[j++];
    }

    glGenVertexArrays (1, &texture_vao);
    glBindVertexArray (texture_vao);
    glGenBuffers (1, &texture_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, texture_vbo);
 
    glBufferData(GL_ARRAY_BUFFER, sizeof(all_vbo_vertices) , &all_vbo_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texture_position_id);
    glVertexAttribPointer(texture_position_id, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(texture_normal_id);
    glVertexAttribPointer(texture_normal_id, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(3*sizeof(float)));
    glEnableVertexAttribArray(texture_texPosition_id);
    glVertexAttribPointer(texture_texPosition_id, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), BUFFER_OFFSET(6*sizeof(float)));
}

void renderTexturedGL(glm::mat4 viewmatrix, glm::mat4 sandtrack_modelviewproject, glm::mat4 bike_modelviewproject, glm::mat4 rider_modelviewproject, std::vector<Light> lights, std::vector<glm::mat4> lightspace_matrices) {
    std::vector<glm::mat4> overall_lightspace;
    float copy_to_uniform_lightspace[4*16]; // max 4 mat4s
    glUseProgram(texture_shader_program);
    glBindVertexArray(texture_vao);
    glUniform1i(texture_shadow_id, 0);
    glUniform1i(texture_sampler_id, 1);
    glUniform1i(texture_uNumLights_id, lights.size());
    glUniformMatrix4fv(texture_uViewMatrix_id, 1, GL_FALSE, glm::value_ptr(viewmatrix));
    glUniform4f(texture_uMaterial_id, 0.75, 0.2, 1.75, 2);
    push_lights_to_uniform(texture_shader_program, lights);

    overall_lightspace.clear();
    for(unsigned int i = 0; i < lights.size(); i++) {
        overall_lightspace.push_back(lightspace_matrices[i] * sandtrack_modelviewproject);
    }
    for(unsigned int i = 0; i < lights.size(); i++) 
        for(unsigned int j = 0; j < 4; j++)
            for(unsigned int k = 0; k < 4; k++)
                copy_to_uniform_lightspace[i*16+j*4+k] = overall_lightspace[i][j][k];
    glUniformMatrix4fv(texture_uModelMatrix_id, 1, GL_FALSE, glm::value_ptr(sandtrack_modelviewproject));
    glUniformMatrix3fv(texture_uNormalMatrix_id, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(sandtrack_modelviewproject)))));
    glUniformMatrix4fv(texture_uLightSpaceMatrix_id, lights.size(), GL_FALSE, copy_to_uniform_lightspace);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sandTrack_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    overall_lightspace.clear();
    for(unsigned int i = 0; i < lights.size(); i++) {
        overall_lightspace.push_back(lightspace_matrices[i] * bike_modelviewproject);
    }
    for(unsigned int i = 0; i < lights.size(); i++) 
        for(unsigned int j = 0; j < 4; j++)
            for(unsigned int k = 0; k < 4; k++)
                copy_to_uniform_lightspace[i*16+j*4+k] = overall_lightspace[i][j][k];
    glUniformMatrix4fv(texture_uModelMatrix_id, 1, GL_FALSE, glm::value_ptr(bike_modelviewproject));
    glUniformMatrix3fv(texture_uNormalMatrix_id, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(bike_modelviewproject)))));
    glUniformMatrix4fv(texture_uLightSpaceMatrix_id, lights.size(), GL_FALSE, copy_to_uniform_lightspace);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bikeHeadlight_texture_id);
    glDrawArrays(GL_TRIANGLES, 6, 6);
    
    overall_lightspace.clear();
    for(unsigned int i = 0; i < lights.size(); i++) {
        overall_lightspace.push_back(lightspace_matrices[i] * rider_modelviewproject);
    }
    for(unsigned int i = 0; i < lights.size(); i++) 
        for(unsigned int j = 0; j < 4; j++)
            for(unsigned int k = 0; k < 4; k++)
                copy_to_uniform_lightspace[i*16+j*4+k] = overall_lightspace[i][j][k];
    glUniformMatrix4fv(texture_uModelMatrix_id, 1, GL_FALSE, glm::value_ptr(rider_modelviewproject));
    glUniformMatrix3fv(texture_uNormalMatrix_id, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(rider_modelviewproject)))));
    glUniformMatrix4fv(texture_uLightSpaceMatrix_id, lights.size(), GL_FALSE, copy_to_uniform_lightspace);
    
    glUniformMatrix4fv(texture_uModelMatrix_id, 1, GL_FALSE, glm::value_ptr(rider_modelviewproject));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, humanoidShirt_texture_id);
    glDrawArrays(GL_TRIANGLES, 12, 6);
}
#endif
