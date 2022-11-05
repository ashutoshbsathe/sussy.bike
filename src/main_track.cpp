#include "main.hpp"
#include "bike.hpp"
#include "rider.hpp"
#include "track.hpp"
#include <GLFW/glfw3.h>
#define MAX_HUMANOID_VBO_BYTES 1024000
#define MAX_BIKE_VBO_BYTES 1024000

HierarchyNode *bike_node, *humanoid_node, *curr_node;
std::vector<AnimationEntity> entities;
int entity_idx = 0;

GLuint track_shader_program;
GLuint track_vbo, track_vao;

GLuint bike_vbo, bike_vao, bike_shader_program, bike_position_id, bike_color_id, bike_uModelViewProjectMatrix_id;
GLuint rider_vbo, rider_vao, rider_shader_program, rider_position_id, rider_color_id, rider_uModelViewProjectMatrix_id;

GLuint track_position_id, track_uModelViewProjectMatrix_id, track_color_id;

glm::mat4 rotation_matrix;
glm::mat4 view_matrix;
glm::mat4 ortho_matrix;
glm::mat4 projection_matrix;
glm::mat4 modelviewproject_matrix;
void trackInitShadersGL(void) {
    std::string vertex_shader_file("generic_vs.glsl");
    std::string fragment_shader_file("generic_fs.glsl");

    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

    track_shader_program = csX75::CreateProgramGL(shaderList);
    track_position_id = glGetAttribLocation(track_shader_program, "vPosition");
    track_color_id = glGetAttribLocation(track_shader_program, "vColor");
    track_uModelViewProjectMatrix_id = glGetUniformLocation(track_shader_program, "uModelViewProjectMatrix");
}
void bikeInitShadersGL(void) {
    std::string vertex_shader_file("generic_vs.glsl");
    std::string fragment_shader_file("generic_fs.glsl");

    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

    bike_shader_program = csX75::CreateProgramGL(shaderList);
    bike_position_id = glGetAttribLocation(bike_shader_program, "vPosition");
    bike_color_id = glGetAttribLocation(bike_shader_program, "vColor");
    bike_uModelViewProjectMatrix_id = glGetUniformLocation(bike_shader_program, "uModelViewProjectMatrix");
}
void riderInitShadersGL(void) {
    std::string vertex_shader_file("generic_vs.glsl");
    std::string fragment_shader_file("generic_fs.glsl");

    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

    rider_shader_program = csX75::CreateProgramGL(shaderList);
    rider_position_id = glGetAttribLocation(rider_shader_program, "vPosition");
    rider_color_id = glGetAttribLocation(rider_shader_program, "vColor");
    rider_uModelViewProjectMatrix_id = glGetUniformLocation(rider_shader_program, "uModelViewProjectMatrix");
}
void trackInitVertexBufferGL(void) {
    //Ask GL for a Vertex Attribute Object (track_vao)
    glGenVertexArrays (1, &track_vao);
    //Set it as the current array to be used by binding it
    glBindVertexArray (track_vao);
    //Ask GL for a Vertex Buffer Object (track_vbo)
    glGenBuffers (1, &track_vbo);
    //Set it as the current buffer to be used by binding it
    glBindBuffer (GL_ARRAY_BUFFER, track_vbo);

    int i;
    // TRACK LOWER
    for(i = 0; i < p_track_lower.triangle_list.size(); i++) {
        sp_tri[9*i] = p_track_lower.triangle_list[i].p1.x;
        sp_tri[9*i+1] = p_track_lower.triangle_list[i].p1.y;
        sp_tri[9*i+2] = p_track_lower.triangle_list[i].p1.z;

        sp_tri[9*i+3] = p_track_lower.triangle_list[i].p2.x;
        sp_tri[9*i+4] = p_track_lower.triangle_list[i].p2.y;
        sp_tri[9*i+5] = p_track_lower.triangle_list[i].p2.z;

        sp_tri[9*i+6] = p_track_lower.triangle_list[i].p3.x;
        sp_tri[9*i+7] = p_track_lower.triangle_list[i].p3.y;
        sp_tri[9*i+8] = p_track_lower.triangle_list[i].p3.z;

        sp_col[9*i] = p_track_lower.triangle_list[i].c.x;
        sp_col[9*i+1] = p_track_lower.triangle_list[i].c.y;
        sp_col[9*i+2] = p_track_lower.triangle_list[i].c.z;

        sp_col[9*i+3] = p_track_lower.triangle_list[i].c.x;
        sp_col[9*i+4] = p_track_lower.triangle_list[i].c.y;
        sp_col[9*i+5] = p_track_lower.triangle_list[i].c.z;

        sp_col[9*i+6] = p_track_lower.triangle_list[i].c.x;
        sp_col[9*i+7] = p_track_lower.triangle_list[i].c.y;
        sp_col[9*i+8] = p_track_lower.triangle_list[i].c.z;
    }

    //  startPlatform
    for(int j = 0; j < startPlatform_triangle_list.size(); j++, i++){
        sp_tri[9*i] = startPlatform_triangle_list[j].p1.x;
        sp_tri[9*i+1] = startPlatform_triangle_list[j].p1.y;
        sp_tri[9*i+2] = startPlatform_triangle_list[j].p1.z;

        sp_tri[9*i+3] = startPlatform_triangle_list[j].p2.x;
        sp_tri[9*i+4] = startPlatform_triangle_list[j].p2.y;
        sp_tri[9*i+5] = startPlatform_triangle_list[j].p2.z;

        sp_tri[9*i+6] = startPlatform_triangle_list[j].p3.x;
        sp_tri[9*i+7] = startPlatform_triangle_list[j].p3.y;
        sp_tri[9*i+8] = startPlatform_triangle_list[j].p3.z;

        sp_col[9*i] = startPlatform_triangle_list[j].c.x;
        sp_col[9*i+1] = startPlatform_triangle_list[j].c.y;
        sp_col[9*i+2] = startPlatform_triangle_list[j].c.z;

        sp_col[9*i+3] = startPlatform_triangle_list[j].c.x;
        sp_col[9*i+4] = startPlatform_triangle_list[j].c.y;
        sp_col[9*i+5] = startPlatform_triangle_list[j].c.z;

        sp_col[9*i+6] = startPlatform_triangle_list[j].c.x;
        sp_col[9*i+7] = startPlatform_triangle_list[j].c.y;
        sp_col[9*i+8] = startPlatform_triangle_list[j].c.z;
    }

    //  startPlatform Connecting triangles
    for(int j = 0; j < startPlatformConnectingTriangles.size(); j++, i++){
        sp_tri[9*i] = startPlatformConnectingTriangles[j].p1.x;
        sp_tri[9*i+1] = startPlatformConnectingTriangles[j].p1.y;
        sp_tri[9*i+2] = startPlatformConnectingTriangles[j].p1.z;

        sp_tri[9*i+3] = startPlatformConnectingTriangles[j].p2.x;
        sp_tri[9*i+4] = startPlatformConnectingTriangles[j].p2.y;
        sp_tri[9*i+5] = startPlatformConnectingTriangles[j].p2.z;

        sp_tri[9*i+6] = startPlatformConnectingTriangles[j].p3.x;
        sp_tri[9*i+7] = startPlatformConnectingTriangles[j].p3.y;
        sp_tri[9*i+8] = startPlatformConnectingTriangles[j].p3.z;

        sp_col[9*i] = startPlatformConnectingTriangles[j].c.x;
        sp_col[9*i+1] = startPlatformConnectingTriangles[j].c.y;
        sp_col[9*i+2] = startPlatformConnectingTriangles[j].c.z;

        sp_col[9*i+3] = startPlatformConnectingTriangles[j].c.x;
        sp_col[9*i+4] = startPlatformConnectingTriangles[j].c.y;
        sp_col[9*i+5] = startPlatformConnectingTriangles[j].c.z;

        sp_col[9*i+6] = startPlatformConnectingTriangles[j].c.x;
        sp_col[9*i+7] = startPlatformConnectingTriangles[j].c.y;
        sp_col[9*i+8] = startPlatformConnectingTriangles[j].c.z;
    }
    //  obstacle1
    for(int j = 0; j < obstacle1_triangle_list.size(); j++, i++){
        sp_tri[9*i] = obstacle1_triangle_list[j].p1.x;
        sp_tri[9*i+1] = obstacle1_triangle_list[j].p1.y;
        sp_tri[9*i+2] = obstacle1_triangle_list[j].p1.z;

        sp_tri[9*i+3] = obstacle1_triangle_list[j].p2.x;
        sp_tri[9*i+4] = obstacle1_triangle_list[j].p2.y;
        sp_tri[9*i+5] = obstacle1_triangle_list[j].p2.z;

        sp_tri[9*i+6] = obstacle1_triangle_list[j].p3.x;
        sp_tri[9*i+7] = obstacle1_triangle_list[j].p3.y;
        sp_tri[9*i+8] = obstacle1_triangle_list[j].p3.z;

        sp_col[9*i] = obstacle1_triangle_list[j].c.x;
        sp_col[9*i+1] = obstacle1_triangle_list[j].c.y;
        sp_col[9*i+2] = obstacle1_triangle_list[j].c.z;

        sp_col[9*i+3] = obstacle1_triangle_list[j].c.x;
        sp_col[9*i+4] = obstacle1_triangle_list[j].c.y;
        sp_col[9*i+5] = obstacle1_triangle_list[j].c.z;

        sp_col[9*i+6] = obstacle1_triangle_list[j].c.x;
        sp_col[9*i+7] = obstacle1_triangle_list[j].c.y;
        sp_col[9*i+8] = obstacle1_triangle_list[j].c.z;
    }

    //  obstacle1 Connecting triangles
    for(int j = 0; j < obstacle1ConnectingTriangles.size(); j++, i++){
        sp_tri[9*i] = obstacle1ConnectingTriangles[j].p1.x;
        sp_tri[9*i+1] = obstacle1ConnectingTriangles[j].p1.y;
        sp_tri[9*i+2] = obstacle1ConnectingTriangles[j].p1.z;

        sp_tri[9*i+3] = obstacle1ConnectingTriangles[j].p2.x;
        sp_tri[9*i+4] = obstacle1ConnectingTriangles[j].p2.y;
        sp_tri[9*i+5] = obstacle1ConnectingTriangles[j].p2.z;

        sp_tri[9*i+6] = obstacle1ConnectingTriangles[j].p3.x;
        sp_tri[9*i+7] = obstacle1ConnectingTriangles[j].p3.y;
        sp_tri[9*i+8] = obstacle1ConnectingTriangles[j].p3.z;

        sp_col[9*i] = obstacle1ConnectingTriangles[j].c.x;
        sp_col[9*i+1] = obstacle1ConnectingTriangles[j].c.y;
        sp_col[9*i+2] = obstacle1ConnectingTriangles[j].c.z;

        sp_col[9*i+3] = obstacle1ConnectingTriangles[j].c.x;
        sp_col[9*i+4] = obstacle1ConnectingTriangles[j].c.y;
        sp_col[9*i+5] = obstacle1ConnectingTriangles[j].c.z;

        sp_col[9*i+6] = obstacle1ConnectingTriangles[j].c.x;
        sp_col[9*i+7] = obstacle1ConnectingTriangles[j].c.y;
        sp_col[9*i+8] = obstacle1ConnectingTriangles[j].c.z;
    }
    //  obstacle2_1
    for(int j = 0; j < obstacle2_1_triangle_list.size(); j++, i++){
        sp_tri[9*i] = obstacle2_1_triangle_list[j].p1.x;
        sp_tri[9*i+1] = obstacle2_1_triangle_list[j].p1.y;
        sp_tri[9*i+2] = obstacle2_1_triangle_list[j].p1.z;

        sp_tri[9*i+3] = obstacle2_1_triangle_list[j].p2.x;
        sp_tri[9*i+4] = obstacle2_1_triangle_list[j].p2.y;
        sp_tri[9*i+5] = obstacle2_1_triangle_list[j].p2.z;

        sp_tri[9*i+6] = obstacle2_1_triangle_list[j].p3.x;
        sp_tri[9*i+7] = obstacle2_1_triangle_list[j].p3.y;
        sp_tri[9*i+8] = obstacle2_1_triangle_list[j].p3.z;

        sp_col[9*i] = obstacle2_1_triangle_list[j].c.x;
        sp_col[9*i+1] = obstacle2_1_triangle_list[j].c.y;
        sp_col[9*i+2] = obstacle2_1_triangle_list[j].c.z;

        sp_col[9*i+3] = obstacle2_1_triangle_list[j].c.x;
        sp_col[9*i+4] = obstacle2_1_triangle_list[j].c.y;
        sp_col[9*i+5] = obstacle2_1_triangle_list[j].c.z;

        sp_col[9*i+6] = obstacle2_1_triangle_list[j].c.x;
        sp_col[9*i+7] = obstacle2_1_triangle_list[j].c.y;
        sp_col[9*i+8] = obstacle2_1_triangle_list[j].c.z;
    }

    //  obstacle2_1 Connecting triangles
    for(int j = 0; j < obstacle2_1ConnectingTriangles.size(); j++, i++){
        sp_tri[9*i] = obstacle2_1ConnectingTriangles[j].p1.x;
        sp_tri[9*i+1] = obstacle2_1ConnectingTriangles[j].p1.y;
        sp_tri[9*i+2] = obstacle2_1ConnectingTriangles[j].p1.z;

        sp_tri[9*i+3] = obstacle2_1ConnectingTriangles[j].p2.x;
        sp_tri[9*i+4] = obstacle2_1ConnectingTriangles[j].p2.y;
        sp_tri[9*i+5] = obstacle2_1ConnectingTriangles[j].p2.z;

        sp_tri[9*i+6] = obstacle2_1ConnectingTriangles[j].p3.x;
        sp_tri[9*i+7] = obstacle2_1ConnectingTriangles[j].p3.y;
        sp_tri[9*i+8] = obstacle2_1ConnectingTriangles[j].p3.z;

        sp_col[9*i] = obstacle2_1ConnectingTriangles[j].c.x;
        sp_col[9*i+1] = obstacle2_1ConnectingTriangles[j].c.y;
        sp_col[9*i+2] = obstacle2_1ConnectingTriangles[j].c.z;

        sp_col[9*i+3] = obstacle2_1ConnectingTriangles[j].c.x;
        sp_col[9*i+4] = obstacle2_1ConnectingTriangles[j].c.y;
        sp_col[9*i+5] = obstacle2_1ConnectingTriangles[j].c.z;

        sp_col[9*i+6] = obstacle2_1ConnectingTriangles[j].c.x;
        sp_col[9*i+7] = obstacle2_1ConnectingTriangles[j].c.y;
        sp_col[9*i+8] = obstacle2_1ConnectingTriangles[j].c.z;
    }

    //  obstacle2_2
    for(int j = 0; j < obstacle2_2_triangle_list.size(); j++, i++){
        sp_tri[9*i] = obstacle2_2_triangle_list[j].p1.x;
        sp_tri[9*i+1] = obstacle2_2_triangle_list[j].p1.y;
        sp_tri[9*i+2] = obstacle2_2_triangle_list[j].p1.z;

        sp_tri[9*i+3] = obstacle2_2_triangle_list[j].p2.x;
        sp_tri[9*i+4] = obstacle2_2_triangle_list[j].p2.y;
        sp_tri[9*i+5] = obstacle2_2_triangle_list[j].p2.z;

        sp_tri[9*i+6] = obstacle2_2_triangle_list[j].p3.x;
        sp_tri[9*i+7] = obstacle2_2_triangle_list[j].p3.y;
        sp_tri[9*i+8] = obstacle2_2_triangle_list[j].p3.z;

        sp_col[9*i] = obstacle2_2_triangle_list[j].c.x;
        sp_col[9*i+1] = obstacle2_2_triangle_list[j].c.y;
        sp_col[9*i+2] = obstacle2_2_triangle_list[j].c.z;

        sp_col[9*i+3] = obstacle2_2_triangle_list[j].c.x;
        sp_col[9*i+4] = obstacle2_2_triangle_list[j].c.y;
        sp_col[9*i+5] = obstacle2_2_triangle_list[j].c.z;

        sp_col[9*i+6] = obstacle2_2_triangle_list[j].c.x;
        sp_col[9*i+7] = obstacle2_2_triangle_list[j].c.y;
        sp_col[9*i+8] = obstacle2_2_triangle_list[j].c.z;
    }

    //  obstacle2_2 Connecting triangles
    for(int j = 0; j < obstacle2_2ConnectingTriangles.size(); j++, i++){
        sp_tri[9*i] = obstacle2_2ConnectingTriangles[j].p1.x;
        sp_tri[9*i+1] = obstacle2_2ConnectingTriangles[j].p1.y;
        sp_tri[9*i+2] = obstacle2_2ConnectingTriangles[j].p1.z;

        sp_tri[9*i+3] = obstacle2_2ConnectingTriangles[j].p2.x;
        sp_tri[9*i+4] = obstacle2_2ConnectingTriangles[j].p2.y;
        sp_tri[9*i+5] = obstacle2_2ConnectingTriangles[j].p2.z;

        sp_tri[9*i+6] = obstacle2_2ConnectingTriangles[j].p3.x;
        sp_tri[9*i+7] = obstacle2_2ConnectingTriangles[j].p3.y;
        sp_tri[9*i+8] = obstacle2_2ConnectingTriangles[j].p3.z;

        sp_col[9*i] = obstacle2_2ConnectingTriangles[j].c.x;
        sp_col[9*i+1] = obstacle2_2ConnectingTriangles[j].c.y;
        sp_col[9*i+2] = obstacle2_2ConnectingTriangles[j].c.z;

        sp_col[9*i+3] = obstacle2_2ConnectingTriangles[j].c.x;
        sp_col[9*i+4] = obstacle2_2ConnectingTriangles[j].c.y;
        sp_col[9*i+5] = obstacle2_2ConnectingTriangles[j].c.z;

        sp_col[9*i+6] = obstacle2_2ConnectingTriangles[j].c.x;
        sp_col[9*i+7] = obstacle2_2ConnectingTriangles[j].c.y;
        sp_col[9*i+8] = obstacle2_2ConnectingTriangles[j].c.z;
    }

    //  obstacle3_1
    for(int j = 0; j < obstacle3_1_triangle_list.size(); j++, i++){
        sp_tri[9*i] = obstacle3_1_triangle_list[j].p1.x;
        sp_tri[9*i+1] = obstacle3_1_triangle_list[j].p1.y;
        sp_tri[9*i+2] = obstacle3_1_triangle_list[j].p1.z;

        sp_tri[9*i+3] = obstacle3_1_triangle_list[j].p2.x;
        sp_tri[9*i+4] = obstacle3_1_triangle_list[j].p2.y;
        sp_tri[9*i+5] = obstacle3_1_triangle_list[j].p2.z;

        sp_tri[9*i+6] = obstacle3_1_triangle_list[j].p3.x;
        sp_tri[9*i+7] = obstacle3_1_triangle_list[j].p3.y;
        sp_tri[9*i+8] = obstacle3_1_triangle_list[j].p3.z;

        sp_col[9*i] = obstacle3_1_triangle_list[j].c.x;
        sp_col[9*i+1] = obstacle3_1_triangle_list[j].c.y;
        sp_col[9*i+2] = obstacle3_1_triangle_list[j].c.z;

        sp_col[9*i+3] = obstacle3_1_triangle_list[j].c.x;
        sp_col[9*i+4] = obstacle3_1_triangle_list[j].c.y;
        sp_col[9*i+5] = obstacle3_1_triangle_list[j].c.z;

        sp_col[9*i+6] = obstacle3_1_triangle_list[j].c.x;
        sp_col[9*i+7] = obstacle3_1_triangle_list[j].c.y;
        sp_col[9*i+8] = obstacle3_1_triangle_list[j].c.z;
    }

    //  obstacle3_1 Connecting triangles
    for(int j = 0; j < obstacle3_1ConnectingTriangles.size(); j++, i++){
        sp_tri[9*i] = obstacle3_1ConnectingTriangles[j].p1.x;
        sp_tri[9*i+1] = obstacle3_1ConnectingTriangles[j].p1.y;
        sp_tri[9*i+2] = obstacle3_1ConnectingTriangles[j].p1.z;

        sp_tri[9*i+3] = obstacle3_1ConnectingTriangles[j].p2.x;
        sp_tri[9*i+4] = obstacle3_1ConnectingTriangles[j].p2.y;
        sp_tri[9*i+5] = obstacle3_1ConnectingTriangles[j].p2.z;

        sp_tri[9*i+6] = obstacle3_1ConnectingTriangles[j].p3.x;
        sp_tri[9*i+7] = obstacle3_1ConnectingTriangles[j].p3.y;
        sp_tri[9*i+8] = obstacle3_1ConnectingTriangles[j].p3.z;

        sp_col[9*i] = obstacle3_1ConnectingTriangles[j].c.x;
        sp_col[9*i+1] = obstacle3_1ConnectingTriangles[j].c.y;
        sp_col[9*i+2] = obstacle3_1ConnectingTriangles[j].c.z;

        sp_col[9*i+3] = obstacle3_1ConnectingTriangles[j].c.x;
        sp_col[9*i+4] = obstacle3_1ConnectingTriangles[j].c.y;
        sp_col[9*i+5] = obstacle3_1ConnectingTriangles[j].c.z;

        sp_col[9*i+6] = obstacle3_1ConnectingTriangles[j].c.x;
        sp_col[9*i+7] = obstacle3_1ConnectingTriangles[j].c.y;
        sp_col[9*i+8] = obstacle3_1ConnectingTriangles[j].c.z;
    }
    
    //  obstacle3_2
    for(int j = 0; j < obstacle3_2_triangle_list.size(); j++, i++){
        sp_tri[9*i] = obstacle3_2_triangle_list[j].p1.x;
        sp_tri[9*i+1] = obstacle3_2_triangle_list[j].p1.y;
        sp_tri[9*i+2] = obstacle3_2_triangle_list[j].p1.z;

        sp_tri[9*i+3] = obstacle3_2_triangle_list[j].p2.x;
        sp_tri[9*i+4] = obstacle3_2_triangle_list[j].p2.y;
        sp_tri[9*i+5] = obstacle3_2_triangle_list[j].p2.z;

        sp_tri[9*i+6] = obstacle3_2_triangle_list[j].p3.x;
        sp_tri[9*i+7] = obstacle3_2_triangle_list[j].p3.y;
        sp_tri[9*i+8] = obstacle3_2_triangle_list[j].p3.z;

        sp_col[9*i] = obstacle3_2_triangle_list[j].c.x;
        sp_col[9*i+1] = obstacle3_2_triangle_list[j].c.y;
        sp_col[9*i+2] = obstacle3_2_triangle_list[j].c.z;

        sp_col[9*i+3] = obstacle3_2_triangle_list[j].c.x;
        sp_col[9*i+4] = obstacle3_2_triangle_list[j].c.y;
        sp_col[9*i+5] = obstacle3_2_triangle_list[j].c.z;

        sp_col[9*i+6] = obstacle3_2_triangle_list[j].c.x;
        sp_col[9*i+7] = obstacle3_2_triangle_list[j].c.y;
        sp_col[9*i+8] = obstacle3_2_triangle_list[j].c.z;
    }

    //  obstacle3_2 Connecting triangles
    for(int j = 0; j < obstacle3_2ConnectingTriangles.size(); j++, i++){
        sp_tri[9*i] = obstacle3_2ConnectingTriangles[j].p1.x;
        sp_tri[9*i+1] = obstacle3_2ConnectingTriangles[j].p1.y;
        sp_tri[9*i+2] = obstacle3_2ConnectingTriangles[j].p1.z;

        sp_tri[9*i+3] = obstacle3_2ConnectingTriangles[j].p2.x;
        sp_tri[9*i+4] = obstacle3_2ConnectingTriangles[j].p2.y;
        sp_tri[9*i+5] = obstacle3_2ConnectingTriangles[j].p2.z;

        sp_tri[9*i+6] = obstacle3_2ConnectingTriangles[j].p3.x;
        sp_tri[9*i+7] = obstacle3_2ConnectingTriangles[j].p3.y;
        sp_tri[9*i+8] = obstacle3_2ConnectingTriangles[j].p3.z;

        sp_col[9*i] = obstacle3_2ConnectingTriangles[j].c.x;
        sp_col[9*i+1] = obstacle3_2ConnectingTriangles[j].c.y;
        sp_col[9*i+2] = obstacle3_2ConnectingTriangles[j].c.z;

        sp_col[9*i+3] = obstacle3_2ConnectingTriangles[j].c.x;
        sp_col[9*i+4] = obstacle3_2ConnectingTriangles[j].c.y;
        sp_col[9*i+5] = obstacle3_2ConnectingTriangles[j].c.z;

        sp_col[9*i+6] = obstacle3_2ConnectingTriangles[j].c.x;
        sp_col[9*i+7] = obstacle3_2ConnectingTriangles[j].c.y;
        sp_col[9*i+8] = obstacle3_2ConnectingTriangles[j].c.z;
    }
    
    //  obstacle4_1
    for(int j = 0; j < obstacle4_1_triangle_list.size(); j++, i++){
        sp_tri[9*i] = obstacle4_1_triangle_list[j].p1.x;
        sp_tri[9*i+1] = obstacle4_1_triangle_list[j].p1.y;
        sp_tri[9*i+2] = obstacle4_1_triangle_list[j].p1.z;

        sp_tri[9*i+3] = obstacle4_1_triangle_list[j].p2.x;
        sp_tri[9*i+4] = obstacle4_1_triangle_list[j].p2.y;
        sp_tri[9*i+5] = obstacle4_1_triangle_list[j].p2.z;

        sp_tri[9*i+6] = obstacle4_1_triangle_list[j].p3.x;
        sp_tri[9*i+7] = obstacle4_1_triangle_list[j].p3.y;
        sp_tri[9*i+8] = obstacle4_1_triangle_list[j].p3.z;

        sp_col[9*i] = obstacle4_1_triangle_list[j].c.x;
        sp_col[9*i+1] = obstacle4_1_triangle_list[j].c.y;
        sp_col[9*i+2] = obstacle4_1_triangle_list[j].c.z;

        sp_col[9*i+3] = obstacle4_1_triangle_list[j].c.x;
        sp_col[9*i+4] = obstacle4_1_triangle_list[j].c.y;
        sp_col[9*i+5] = obstacle4_1_triangle_list[j].c.z;

        sp_col[9*i+6] = obstacle4_1_triangle_list[j].c.x;
        sp_col[9*i+7] = obstacle4_1_triangle_list[j].c.y;
        sp_col[9*i+8] = obstacle4_1_triangle_list[j].c.z;
    }

    //  obstacle4_1 Connecting triangles
    for(int j = 0; j < obstacle4_1ConnectingTriangles.size(); j++, i++){
        sp_tri[9*i] = obstacle4_1ConnectingTriangles[j].p1.x;
        sp_tri[9*i+1] = obstacle4_1ConnectingTriangles[j].p1.y;
        sp_tri[9*i+2] = obstacle4_1ConnectingTriangles[j].p1.z;

        sp_tri[9*i+3] = obstacle4_1ConnectingTriangles[j].p2.x;
        sp_tri[9*i+4] = obstacle4_1ConnectingTriangles[j].p2.y;
        sp_tri[9*i+5] = obstacle4_1ConnectingTriangles[j].p2.z;

        sp_tri[9*i+6] = obstacle4_1ConnectingTriangles[j].p3.x;
        sp_tri[9*i+7] = obstacle4_1ConnectingTriangles[j].p3.y;
        sp_tri[9*i+8] = obstacle4_1ConnectingTriangles[j].p3.z;

        sp_col[9*i] = obstacle4_1ConnectingTriangles[j].c.x;
        sp_col[9*i+1] = obstacle4_1ConnectingTriangles[j].c.y;
        sp_col[9*i+2] = obstacle4_1ConnectingTriangles[j].c.z;

        sp_col[9*i+3] = obstacle4_1ConnectingTriangles[j].c.x;
        sp_col[9*i+4] = obstacle4_1ConnectingTriangles[j].c.y;
        sp_col[9*i+5] = obstacle4_1ConnectingTriangles[j].c.z;

        sp_col[9*i+6] = obstacle4_1ConnectingTriangles[j].c.x;
        sp_col[9*i+7] = obstacle4_1ConnectingTriangles[j].c.y;
        sp_col[9*i+8] = obstacle4_1ConnectingTriangles[j].c.z;
    }

    //  obstacle4_2
    for(int j = 0; j < obstacle4_2_triangle_list.size(); j++, i++){
        sp_tri[9*i] = obstacle4_2_triangle_list[j].p1.x;
        sp_tri[9*i+1] = obstacle4_2_triangle_list[j].p1.y;
        sp_tri[9*i+2] = obstacle4_2_triangle_list[j].p1.z;

        sp_tri[9*i+3] = obstacle4_2_triangle_list[j].p2.x;
        sp_tri[9*i+4] = obstacle4_2_triangle_list[j].p2.y;
        sp_tri[9*i+5] = obstacle4_2_triangle_list[j].p2.z;

        sp_tri[9*i+6] = obstacle4_2_triangle_list[j].p3.x;
        sp_tri[9*i+7] = obstacle4_2_triangle_list[j].p3.y;
        sp_tri[9*i+8] = obstacle4_2_triangle_list[j].p3.z;

        sp_col[9*i] = obstacle4_2_triangle_list[j].c.x;
        sp_col[9*i+1] = obstacle4_2_triangle_list[j].c.y;
        sp_col[9*i+2] = obstacle4_2_triangle_list[j].c.z;

        sp_col[9*i+3] = obstacle4_2_triangle_list[j].c.x;
        sp_col[9*i+4] = obstacle4_2_triangle_list[j].c.y;
        sp_col[9*i+5] = obstacle4_2_triangle_list[j].c.z;

        sp_col[9*i+6] = obstacle4_2_triangle_list[j].c.x;
        sp_col[9*i+7] = obstacle4_2_triangle_list[j].c.y;
        sp_col[9*i+8] = obstacle4_2_triangle_list[j].c.z;
    }

    //  obstacle4_2 Connecting triangles
    for(int j = 0; j < obstacle4_2ConnectingTriangles.size(); j++, i++){
        sp_tri[9*i] = obstacle4_2ConnectingTriangles[j].p1.x;
        sp_tri[9*i+1] = obstacle4_2ConnectingTriangles[j].p1.y;
        sp_tri[9*i+2] = obstacle4_2ConnectingTriangles[j].p1.z;

        sp_tri[9*i+3] = obstacle4_2ConnectingTriangles[j].p2.x;
        sp_tri[9*i+4] = obstacle4_2ConnectingTriangles[j].p2.y;
        sp_tri[9*i+5] = obstacle4_2ConnectingTriangles[j].p2.z;

        sp_tri[9*i+6] = obstacle4_2ConnectingTriangles[j].p3.x;
        sp_tri[9*i+7] = obstacle4_2ConnectingTriangles[j].p3.y;
        sp_tri[9*i+8] = obstacle4_2ConnectingTriangles[j].p3.z;

        sp_col[9*i] = obstacle4_2ConnectingTriangles[j].c.x;
        sp_col[9*i+1] = obstacle4_2ConnectingTriangles[j].c.y;
        sp_col[9*i+2] = obstacle4_2ConnectingTriangles[j].c.z;

        sp_col[9*i+3] = obstacle4_2ConnectingTriangles[j].c.x;
        sp_col[9*i+4] = obstacle4_2ConnectingTriangles[j].c.y;
        sp_col[9*i+5] = obstacle4_2ConnectingTriangles[j].c.z;

        sp_col[9*i+6] = obstacle4_2ConnectingTriangles[j].c.x;
        sp_col[9*i+7] = obstacle4_2ConnectingTriangles[j].c.y;
        sp_col[9*i+8] = obstacle4_2ConnectingTriangles[j].c.z;
    }

    //  obstacle5_1
    for(int j = 0; j < obstacle5_1_triangle_list.size(); j++, i++){
        sp_tri[9*i] = obstacle5_1_triangle_list[j].p1.x;
        sp_tri[9*i+1] = obstacle5_1_triangle_list[j].p1.y;
        sp_tri[9*i+2] = obstacle5_1_triangle_list[j].p1.z;

        sp_tri[9*i+3] = obstacle5_1_triangle_list[j].p2.x;
        sp_tri[9*i+4] = obstacle5_1_triangle_list[j].p2.y;
        sp_tri[9*i+5] = obstacle5_1_triangle_list[j].p2.z;

        sp_tri[9*i+6] = obstacle5_1_triangle_list[j].p3.x;
        sp_tri[9*i+7] = obstacle5_1_triangle_list[j].p3.y;
        sp_tri[9*i+8] = obstacle5_1_triangle_list[j].p3.z;

        sp_col[9*i] = obstacle5_1_triangle_list[j].c.x;
        sp_col[9*i+1] = obstacle5_1_triangle_list[j].c.y;
        sp_col[9*i+2] = obstacle5_1_triangle_list[j].c.z;

        sp_col[9*i+3] = obstacle5_1_triangle_list[j].c.x;
        sp_col[9*i+4] = obstacle5_1_triangle_list[j].c.y;
        sp_col[9*i+5] = obstacle5_1_triangle_list[j].c.z;

        sp_col[9*i+6] = obstacle5_1_triangle_list[j].c.x;
        sp_col[9*i+7] = obstacle5_1_triangle_list[j].c.y;
        sp_col[9*i+8] = obstacle5_1_triangle_list[j].c.z;
    }

    //  obstacle5_1 Connecting triangles
    for(int j = 0; j < obstacle5_1ConnectingTriangles.size(); j++, i++){
        sp_tri[9*i] = obstacle5_1ConnectingTriangles[j].p1.x;
        sp_tri[9*i+1] = obstacle5_1ConnectingTriangles[j].p1.y;
        sp_tri[9*i+2] = obstacle5_1ConnectingTriangles[j].p1.z;

        sp_tri[9*i+3] = obstacle5_1ConnectingTriangles[j].p2.x;
        sp_tri[9*i+4] = obstacle5_1ConnectingTriangles[j].p2.y;
        sp_tri[9*i+5] = obstacle5_1ConnectingTriangles[j].p2.z;

        sp_tri[9*i+6] = obstacle5_1ConnectingTriangles[j].p3.x;
        sp_tri[9*i+7] = obstacle5_1ConnectingTriangles[j].p3.y;
        sp_tri[9*i+8] = obstacle5_1ConnectingTriangles[j].p3.z;

        sp_col[9*i] = obstacle5_1ConnectingTriangles[j].c.x;
        sp_col[9*i+1] = obstacle5_1ConnectingTriangles[j].c.y;
        sp_col[9*i+2] = obstacle5_1ConnectingTriangles[j].c.z;

        sp_col[9*i+3] = obstacle5_1ConnectingTriangles[j].c.x;
        sp_col[9*i+4] = obstacle5_1ConnectingTriangles[j].c.y;
        sp_col[9*i+5] = obstacle5_1ConnectingTriangles[j].c.z;

        sp_col[9*i+6] = obstacle5_1ConnectingTriangles[j].c.x;
        sp_col[9*i+7] = obstacle5_1ConnectingTriangles[j].c.y;
        sp_col[9*i+8] = obstacle5_1ConnectingTriangles[j].c.z;
    }
    //  obstacle5_2
    for(int j = 0; j < obstacle5_2_triangle_list.size(); j++, i++){
        sp_tri[9*i] = obstacle5_2_triangle_list[j].p1.x;
        sp_tri[9*i+1] = obstacle5_2_triangle_list[j].p1.y;
        sp_tri[9*i+2] = obstacle5_2_triangle_list[j].p1.z;

        sp_tri[9*i+3] = obstacle5_2_triangle_list[j].p2.x;
        sp_tri[9*i+4] = obstacle5_2_triangle_list[j].p2.y;
        sp_tri[9*i+5] = obstacle5_2_triangle_list[j].p2.z;

        sp_tri[9*i+6] = obstacle5_2_triangle_list[j].p3.x;
        sp_tri[9*i+7] = obstacle5_2_triangle_list[j].p3.y;
        sp_tri[9*i+8] = obstacle5_2_triangle_list[j].p3.z;

        sp_col[9*i] = obstacle5_2_triangle_list[j].c.x;
        sp_col[9*i+1] = obstacle5_2_triangle_list[j].c.y;
        sp_col[9*i+2] = obstacle5_2_triangle_list[j].c.z;

        sp_col[9*i+3] = obstacle5_2_triangle_list[j].c.x;
        sp_col[9*i+4] = obstacle5_2_triangle_list[j].c.y;
        sp_col[9*i+5] = obstacle5_2_triangle_list[j].c.z;

        sp_col[9*i+6] = obstacle5_2_triangle_list[j].c.x;
        sp_col[9*i+7] = obstacle5_2_triangle_list[j].c.y;
        sp_col[9*i+8] = obstacle5_2_triangle_list[j].c.z;
    }

    //  obstacle5_2 Connecting triangles
    for(int j = 0; j < obstacle5_2ConnectingTriangles.size(); j++, i++){
        sp_tri[9*i] = obstacle5_2ConnectingTriangles[j].p1.x;
        sp_tri[9*i+1] = obstacle5_2ConnectingTriangles[j].p1.y;
        sp_tri[9*i+2] = obstacle5_2ConnectingTriangles[j].p1.z;

        sp_tri[9*i+3] = obstacle5_2ConnectingTriangles[j].p2.x;
        sp_tri[9*i+4] = obstacle5_2ConnectingTriangles[j].p2.y;
        sp_tri[9*i+5] = obstacle5_2ConnectingTriangles[j].p2.z;

        sp_tri[9*i+6] = obstacle5_2ConnectingTriangles[j].p3.x;
        sp_tri[9*i+7] = obstacle5_2ConnectingTriangles[j].p3.y;
        sp_tri[9*i+8] = obstacle5_2ConnectingTriangles[j].p3.z;

        sp_col[9*i] = obstacle5_2ConnectingTriangles[j].c.x;
        sp_col[9*i+1] = obstacle5_2ConnectingTriangles[j].c.y;
        sp_col[9*i+2] = obstacle5_2ConnectingTriangles[j].c.z;

        sp_col[9*i+3] = obstacle5_2ConnectingTriangles[j].c.x;
        sp_col[9*i+4] = obstacle5_2ConnectingTriangles[j].c.y;
        sp_col[9*i+5] = obstacle5_2ConnectingTriangles[j].c.z;

        sp_col[9*i+6] = obstacle5_2ConnectingTriangles[j].c.x;
        sp_col[9*i+7] = obstacle5_2ConnectingTriangles[j].c.y;
        sp_col[9*i+8] = obstacle5_2ConnectingTriangles[j].c.z;
    }

    //Copy the points into the current buffer - 9 float values, obstacle1 pointer and static data
    glBufferData (GL_ARRAY_BUFFER, sp_n_tris * 3 * 3 * sizeof (float) + sp_n_tris * 3 * 3 * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sp_n_tris * 3 * 3 * sizeof(float), sp_tri);
    glBufferSubData(GL_ARRAY_BUFFER, sp_n_tris * 3 * 3 * sizeof(float), sp_n_tris * 3 * 3 * sizeof(float), sp_col);

    //Enable the vertex attribute
    glEnableVertexAttribArray (track_position_id);
    //This the layout of our first vertex buffer
    //"0" means define the layout for attribute number 0. "3" means that the variables are vec3 made from every 3 floats
    glVertexAttribPointer (track_position_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    glEnableVertexAttribArray(track_color_id);
    glVertexAttribPointer(track_color_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sp_n_tris * 3 * 3 * sizeof(float)));
}
void riderInitVertexBufferGL(void) {
    //Ask GL for a Vertex Attribute Object (rider_vao)
    glGenVertexArrays (1, &rider_vao);
    //Set it as the current array to be used by binding it
    glBindVertexArray (rider_vao);
    //Ask GL for a Vertex Buffer Object (rider_vbo)
    glGenBuffers (1, &rider_vbo);
    //Set it as the current buffer to be used by binding it
    glBindBuffer (GL_ARRAY_BUFFER, rider_vbo);

    glBufferData (GL_ARRAY_BUFFER, MAX_HUMANOID_VBO_BYTES, NULL, GL_STATIC_DRAW);

    humanoid_node = build_humanoid(rider_vao, rider_vbo, rider_uModelViewProjectMatrix_id);
    humanoid_node->prepare_vbo();
    auto e = AnimationEntity("rider", humanoid_node);
    e.read_params_from_file("params_rider.txt");
    entities.push_back(e);
    curr_node = humanoid_node;
    //Enable the vertex attribute
    glEnableVertexAttribArray (rider_position_id);
    glVertexAttribPointer (rider_position_id, 3, GL_FLOAT, GL_FALSE, 3 * 2 * sizeof(float), BUFFER_OFFSET(0));

    glEnableVertexAttribArray(rider_color_id);
    glVertexAttribPointer(rider_color_id, 3, GL_FLOAT, GL_FALSE, 3 * 2 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
}
void bikeInitVertexBufferGL(void) {
    //Ask GL for a Vertex Attribute Object (bike_vao)
    glGenVertexArrays (1, &bike_vao);
    //Set it as the current array to be used by binding it
    glBindVertexArray (bike_vao);
    //Ask GL for a Vertex Buffer Object (bike_vbo)
    glGenBuffers (1, &bike_vbo);
    //Set it as the current buffer to be used by binding it
    glBindBuffer (GL_ARRAY_BUFFER, bike_vbo);

    glBufferData (GL_ARRAY_BUFFER, MAX_HUMANOID_VBO_BYTES, NULL, GL_STATIC_DRAW);

    bike_node = build_bike(bike_vao, bike_vbo, bike_uModelViewProjectMatrix_id);
    bike_node->prepare_vbo();
    auto e = AnimationEntity("bike", bike_node);
    e.read_params_from_file("params_bike.txt");
    entities.push_back(e);

    //Enable the vertex attribute
    glEnableVertexAttribArray (bike_position_id);
    glVertexAttribPointer (bike_position_id, 3, GL_FLOAT, GL_FALSE, 3 * 2 * sizeof(float), BUFFER_OFFSET(0));

    glEnableVertexAttribArray(bike_color_id);
    glVertexAttribPointer(bike_color_id, 3, GL_FLOAT, GL_FALSE, 3 * 2 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
}
void renderGL(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    rotation_matrix = glm::rotate(glm::mat4(1), xrot, glm::vec3(1, 0, 0));
    rotation_matrix = glm::rotate(rotation_matrix, yrot, glm::vec3(0, 1, 0));
    rotation_matrix = glm::rotate(rotation_matrix, zrot, glm::vec3(0, 0, 1));

    view_matrix = glm::lookAt(glm::vec3(0.0,0.0,VIEW_PADDING*DRAW_MIN),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));

    ortho_matrix = glm::ortho(
                       VIEW_PADDING * DRAW_MIN, VIEW_PADDING * DRAW_MAX,
                       VIEW_PADDING * DRAW_MIN, VIEW_PADDING * DRAW_MAX,
                       10 * VIEW_PADDING * DRAW_MIN, 10 * VIEW_PADDING * DRAW_MAX
                   );
    projection_matrix = glm::frustum(-1,1,-1,1,1,10);
    
    if(persp) 
        modelviewproject_matrix = projection_matrix * view_matrix;
    else
        modelviewproject_matrix = ortho_matrix * view_matrix;

    modelviewproject_matrix = modelviewproject_matrix * rotation_matrix;
    
    glUseProgram(rider_shader_program);
    glBindVertexArray(rider_vao);

    viewproject = modelviewproject_matrix;
    // hierarchy_matrix_stack = glm::mat4(1);
    hierarchy_matrix_stack = glm::translate(glm::mat4(1), glm::vec3(12000, 2750, -4500)) * glm::rotate(glm::mat4(1), (glm::mediump_float)(M_PI/2 + M_PI/3), glm::vec3(0, 1, 0));
    humanoid_node->render_dag();

    glUseProgram(bike_shader_program);
    glBindVertexArray(bike_vao);

    viewproject = modelviewproject_matrix;
    // hierarchy_matrix_stack = glm::mat4(1);
    hierarchy_matrix_stack = glm::translate(glm::mat4(1), glm::vec3(12000, 2750, -4500))* glm::rotate(glm::mat4(1), (glm::mediump_float)(M_PI/2 + M_PI/3), glm::vec3(0, 1, 0));
    bike_node->render_dag();

    
    // Draw the track
    glUseProgram(track_shader_program);
    glBindVertexArray(track_vao);
    glUniformMatrix4fv(track_uModelViewProjectMatrix_id, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix)); // value_ptr needed for proper pointer conversion
    glDrawArrays(GL_TRIANGLES, 0, sp_n_tris * 3);
}

int main(int argc, char** argv) {
    //! The pointer to the GLFW window
    GLFWwindow* window;

    //! Setting up the GLFW Error callback
    glfwSetErrorCallback(csX75::error_callback);

    //! Initialize GLFW
    if (!glfwInit())
        return -1;

    //We want OpenGL 4.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //This is for MacOSX - can be omitted otherwise
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //We don't want the old OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //! Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "CS475/CS675 OpenGL Framework", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    //! Make the window's context current
    glfwMakeContextCurrent(window);

    //Initialize GLEW
    //Turn this on to get Shader based OpenGL
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        //Problem: glewInit failed, something is seriously wrong.
        std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

    //Print and see what context got enabled
  	std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  	std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  	std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  	std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

    //Keyboard Callback
    glfwSetKeyCallback(window, csX75::key_callback);
    //Framebuffer resize callback
    glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //Initialize GL state
    csX75::initGL();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    trackInitShadersGL();
    riderInitShadersGL();
    bikeInitShadersGL();
    trackInitVertexBufferGL();
    riderInitVertexBufferGL();
    bikeInitVertexBufferGL();
    // Loop until the user closes the window
    while (glfwWindowShouldClose(window) == 0) {

        // Render here
        renderGL();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
