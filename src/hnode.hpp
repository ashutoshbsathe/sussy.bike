#ifndef _HNODE_HPP_
#define _HNODE_HPP_
#include "triangle.hpp"
#include "spprism.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "gl_framework.hpp"

struct HierarchyNode {
    std::string name;
    HierarchyNode *parent;
    std::vector<HierarchyNode *> children;
 
    /* line can also be represented with a Triangle as p1,p2,c */
    std::vector<Triangle> triangle_list, line_list;
    float *vbo_copy;
    /*
     * `local_transform` aligns the objects wrt parent
     * `private_transform` are object specific transforms if any  should be set to identity otherwise
     */
    glm::mat4 local_transform, private_transform, dof_transform;
    
    unsigned int n_dof;
    std::vector<float> dof_deltas;
    std::vector<std::pair<float, float>> dof_limits;
    /*
     * Max 6 degrees of freedom
     * First 3 are always rotations and next 3 are always translations
     * You may require rotations/translations around weird axes hence storing 
     * the glm::vec3 params as well
     */
    std::vector<std::pair<float, glm::vec3>> dof_params;

    GLuint vbo_offset; // how many "points" deep are we in the VBO
    std::map<std::string, GLuint> gl_info;

    bool draw_triangle, draw_line;

    HierarchyNode();
    HierarchyNode(StackedPolyPrism p);
    HierarchyNode(std::string fname);
    HierarchyNode(std::string name, std::vector<Triangle> triangle_list, std::vector<Triangle> line_list);
    ~HierarchyNode();
    void setGLInfo(std::map<std::string, GLuint> gl_info);
    void make_rigid();
    void init_default_dof_transform();
    void update_dof_transform();
    void prepare_vbo();
    void render(bool shadowmap);
    void render_dag(bool shadowmap);
};

extern glm::mat4 hnode_viewproject;
extern glm::mat4 hnode_viewmatrix;
extern glm::mat4 hnode_lightspacematrix;
extern glm::mat4 hnode_hierarchy_matrix_stack;
extern GLuint depthMap;
void global_to_local(HierarchyNode *n);
void add_edge(HierarchyNode *parent, HierarchyNode *child, unsigned int *next_available_vbo_offset);
#endif
