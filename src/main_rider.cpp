#include "main.hpp"
#include "rider.hpp"
#include "stb_image_write.h"
#include <GLFW/glfw3.h>

#define MAX_HUMANOID_VBO_BYTES 1024000

GLuint shader_program, vbo, vao, uModelViewProjectMatrix_id, uNormalMatrix_id, uViewMatrix_id, uLightSpaceMatrix_id, uShadowMap_id, position_id, color_id, normal_id;

GLuint shadow_shader_program, shadow_uModelMatrix_id, shadow_uLightSpaceMatrix_id, shadow_shadowMap_id;

GLuint depthMapFBO, depthMap_width = 1024, depthMap_height = 1024;

glm::mat4 view_matrix;
glm::mat4 ortho_matrix;
glm::mat4 projection_matrix;
glm::mat4 modelviewproject_matrix;
glm::mat4 lightspace_matrix;
glm::mat4 rotation_matrix;
glm::mat3 normal_matrix;

HierarchyNode *humanoid, *curr_node;
std::vector<AnimationEntity> entities;
int entity_idx = 0;

void initShadersGL(void) {
    std::string vertex_shader_file("shading_vs.glsl");
    std::string fragment_shader_file("shading_fs.glsl");

    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

    shader_program = csX75::CreateProgramGL(shaderList);
    position_id = glGetAttribLocation(shader_program, "vPosition");
    color_id = glGetAttribLocation(shader_program, "vColor");
    normal_id = glGetAttribLocation(shader_program, "vNormal");
    uModelViewProjectMatrix_id = glGetUniformLocation(shader_program, "uModelViewProjectMatrix");
    uNormalMatrix_id = glGetUniformLocation(shader_program, "uNormalMatrix");
    uViewMatrix_id = glGetUniformLocation(shader_program, "uViewMatrix");
    uLightSpaceMatrix_id = glGetAttribLocation(shader_program, "uLightSpaceMatrix");
    uShadowMap_id = glGetAttribLocation(shader_program, "shadowMap");

    /* Shadow Mapping */
    shaderList.clear();
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, "shadow_mapping_vs.glsl"));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, "shadow_mapping_fs.glsl"));
    
    shadow_shader_program = csX75::CreateProgramGL(shaderList);
    shadow_uLightSpaceMatrix_id = glGetAttribLocation(shadow_shader_program, "uLightSpaceMatrix");
    shadow_uModelMatrix_id = glGetAttribLocation(shadow_shader_program, "uModelMatrix");
}

void initVertexBufferGL(void) {
    //Ask GL for a Vertex Attribute Object (vao)
    glGenVertexArrays (1, &vao);
    //Set it as the current array to be used by binding it
    glBindVertexArray (vao);
    //Ask GL for a Vertex Buffer Object (vbo)
    glGenBuffers (1, &vbo);
    //Set it as the current buffer to be used by binding it
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, MAX_HUMANOID_VBO_BYTES, NULL, GL_STATIC_DRAW);
    
    humanoid = build_humanoid(vao, vbo, uModelViewProjectMatrix_id, uNormalMatrix_id, uViewMatrix_id, uLightSpaceMatrix_id, uShadowMap_id);
    humanoid->prepare_vbo();
    entities.push_back(AnimationEntity("standalone_rider", humanoid));
    curr_node = humanoid;
    std::cout << "VBO successfully initialized\n";

    // Enable the vertex attribute
    // Excellent answer -- https://stackoverflow.com/a/39684775
    glEnableVertexAttribArray (position_id);
    glVertexAttribPointer (position_id, 3, GL_FLOAT, GL_FALSE, 3 * 3 * sizeof(float), BUFFER_OFFSET(0));

    glEnableVertexAttribArray(color_id);
    glVertexAttribPointer(color_id, 3, GL_FLOAT, GL_FALSE, 3 * 3 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));

    glEnableVertexAttribArray(normal_id);
    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 3 * 3 * sizeof(float), BUFFER_OFFSET(3 * 2 * sizeof(float)));

    /*
     * Init Depth Buffer for Shadow Mapping
     */
    glGenFramebuffers(1, &depthMapFBO);
    
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMap_width, depthMap_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderGL(void) {
    /* Shadow Mapping */
 
    view_matrix = glm::lookAt(glm::vec3(0, 0, 1060),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
    
    projection_matrix = glm::ortho(
                       1060.0f, -1060.0f,
                       1060.0f, -1060.0f,
                       -2120.0f, 2120.0f
                   );
    
    lightspace_matrix = projection_matrix * view_matrix;
    /*
    std::cout << "Light = ";
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << lightspace_matrix[i][j] << " ";
        }
    }
    std::cout << "\n";
    auto transformed_origin = lightspace_matrix * glm::vec4(0, 0, 0, 1);
    std::cout << "O = {" << transformed_origin.x << " " << transformed_origin.y << " " << transformed_origin.z << "}\n";
    auto transformed_x_pos_one = lightspace_matrix * glm::vec4(1, 0, 1, 1);
    std::cout << "+1[X] = {" << transformed_x_pos_one.x << " " << transformed_x_pos_one.y << " " << transformed_x_pos_one.z << "}\n";
    auto transformed_x_neg_one = lightspace_matrix * glm::vec4(-1, 0, 1, 1);
    std::cout << "-1[X] = {" << transformed_x_neg_one.x << " " << transformed_x_neg_one.y << " " << transformed_x_neg_one.z << "}\n";
    auto transformed_y_pos_one = lightspace_matrix * glm::vec4(0, 1, -1, 1);
    std::cout << "+1[Y] = {" << transformed_y_pos_one.x << " " << transformed_y_pos_one.y << " " << transformed_y_pos_one.z << "}\n";
    auto transformed_y_neg_one = lightspace_matrix * glm::vec4(0, -1, -1, 1);
    std::cout << "-1[Y] = {" << transformed_y_neg_one.x << " " << transformed_y_neg_one.y << " " << transformed_y_neg_one.z << "}\n";
    exit(0);
    */
    glUseProgram(shadow_shader_program);
    glBindVertexArray(vao);
 
    viewproject = lightspace_matrix;
    viewmatrix = lightspace_matrix;
    lightspacematrix = lightspace_matrix;
    hierarchy_matrix_stack = glm::mat4(1);

    glViewport(0, 0, depthMap_width, depthMap_height);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    humanoid->render_dag();   

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    /* Normal rendering */
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

    if(false) 
        modelviewproject_matrix = projection_matrix * view_matrix;
    else
        modelviewproject_matrix = ortho_matrix * view_matrix;

    glUseProgram(shader_program);

    modelviewproject_matrix *= rotation_matrix;

    normal_matrix = glm::transpose(glm::inverse(glm::mat3(modelviewproject_matrix)));
    glBindVertexArray(vao);

    viewproject = modelviewproject_matrix;
    viewmatrix = modelviewproject_matrix;
    normalmatrix = normal_matrix;
    lightspacematrix = lightspace_matrix;
    hierarchy_matrix_stack = glm::mat4(1);
    
    /*
    std::cout << "Light = ";
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << lightspacematrix[i][j] << " ";
        }
    }
    std::cout << "\n";

    std::cout << "ViewProject = ";
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << viewproject[i][j] << " ";
        }
    }
    std::cout << "\n---------------------------\n";
    */

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    humanoid->render_dag();
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
    /*
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    */
    glDisable(GL_BLEND); 
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    initShadersGL();
    initVertexBufferGL();
    
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

