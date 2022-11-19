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

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define TORSO_2(X) ((X)->children[0]) 
#define NECK(X) ((TORSO_2(X))->children[0])
#define HEAD(X) ((NECK(X))->children[0])
#define HEAD_GLOBAL_TRANSFORM(X) (X->local_transform * X->dof_transform * TORSO_2(X)->local_transform * TORSO_2(X)->dof_transform * NECK(X)->local_transform * NECK(X)->dof_transform * HEAD(X)->local_transform * HEAD(X)->dof_transform)

float xrot = 0, yrot = 0, zrot = 0, rotamount = M_PI/30.0, light_x = 0, light_y = 0, light_z = 0, light_moveamount = M_PI/303.0, xmove = 0, ymove = 0, zmove = 0, moveamount = 50;

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
#endif
