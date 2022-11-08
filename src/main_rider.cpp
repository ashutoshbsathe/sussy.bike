#include "main.hpp"
#include "rider.hpp"
#include "stb_image_write.h"
#include <GLFW/glfw3.h>

#define MAX_HUMANOID_VBO_BYTES 1024000

GLuint shader_program, vbo, vao, uModelViewProjectMatrix_id, uNormalMatrix_id, uViewMatrix_id, uLightSpaceMatrix_id, uShadowMap_id, position_id, color_id, normal_id;

GLuint shadow_shader_program, shadow_vao, shadow_position_id, shadow_color_id, shadow_uModelMatrix_id, shadow_uLightSpaceMatrix_id;

std::ofstream fout;
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
    shadow_position_id = glGetAttribLocation(shadow_shader_program, "vPosition");   // WHY it is using 'vPosition'?
    shadow_color_id = glGetAttribLocation(shadow_shader_program, "vColor");
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
    
    humanoid = build_humanoid(vao, vbo, uModelViewProjectMatrix_id, uNormalMatrix_id, uViewMatrix_id, uLightSpaceMatrix_id, uShadowMap_id, shadow_uLightSpaceMatrix_id, shadow_uModelMatrix_id);
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
    
    std::cout << "Normal rendering attributes enabled\n";
    //Ask GL for a Vertex Attribute Object (vao)
    glGenVertexArrays (1, &shadow_vao);
    //Set it as the current array to be used by binding it
    glBindVertexArray (shadow_vao);
    glBindBuffer (GL_ARRAY_BUFFER, vbo); // shared VBO
    glEnableVertexAttribArray (shadow_position_id);
    glVertexAttribPointer (shadow_position_id, 3, GL_FLOAT, GL_FALSE, 3 * 3 * sizeof(float), BUFFER_OFFSET(0));
    /*
     * Init Depth Buffer for Shadow Mapping
     */
    GLint error = glGetError();
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap); 

    error = glGetError();
    std::cout << "After generating framebuffer and texture " << error << ", " << glewGetErrorString(error) << "\n";
 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMap_width, depthMap_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    
    std::cout << "FBO status: " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << "\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
   
    glBindTexture(GL_TEXTURE_2D, 0);
    error = glGetError();
    std::cout << "At the end of initVertexBufferGL, " << error << ", " << glewGetErrorString(error) << "\n";
}

void renderGL(void) {
    /* Shadow Mapping */
    GLint error = glGetError();
    std::cout << "At the beginning of renderGL, glError = " << error << ", " << glewGetErrorString(error) << "\n";
    view_matrix = glm::lookAt(glm::vec3(0, 0, -VIEW_PADDING*DRAW_MIN),glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0));
    
    projection_matrix = glm::ortho(
                       VIEW_PADDING * DRAW_MIN, VIEW_PADDING * DRAW_MAX,
                       VIEW_PADDING * DRAW_MIN, VIEW_PADDING * DRAW_MAX,
                       10 * VIEW_PADDING * DRAW_MIN, 10 * VIEW_PADDING * DRAW_MAX
                   );
    
    lightspace_matrix = projection_matrix * view_matrix;
    error = glGetError();
    std::cout << "Before shadowmap computation, glError = " << error << ", " << glewGetErrorString(error) << "\n";
    glBindVertexArray(shadow_vao);
    glUseProgram(shadow_shader_program);    // Somehow 'shadow_shader_program' is not linked.
 
    viewproject = lightspace_matrix;
    viewmatrix = lightspace_matrix;
    lightspacematrix = lightspace_matrix;
    hierarchy_matrix_stack = glm::mat4(1);

    glViewport(0, 0, depthMap_width, depthMap_height);
    //glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    //glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    humanoid->render_dag(true);   
    error = glGetError();
    std::cout << "After dag rendering, glError = " << error << ", " << glewGetErrorString(error) << "\n";
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    error = glGetError();
    std::cout << "After breaking framebuffer binding, glError = " << error << ", " << glewGetErrorString(error) << "\n";
    return;
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
    
    std::cout << "Modelviewproject = ";
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << modelviewproject_matrix[i][j] << " ";
        }
    }
    std::cout << "\n";
    transformed_origin = modelviewproject_matrix * glm::vec4(0, 0, 0, 1);
    std::cout << "O = {" << transformed_origin.x << " " << transformed_origin.y << " " << transformed_origin.z << "}\n";
    transformed_x_pos_one = modelviewproject_matrix * glm::vec4(1, 0, 1, 1);
    std::cout << "+1[X] = {" << transformed_x_pos_one.x << " " << transformed_x_pos_one.y << " " << transformed_x_pos_one.z << "}\n";
    transformed_x_neg_one = modelviewproject_matrix * glm::vec4(-1, 0, 1, 1);
    std::cout << "-1[X] = {" << transformed_x_neg_one.x << " " << transformed_x_neg_one.y << " " << transformed_x_neg_one.z << "}\n";
    transformed_y_pos_one = modelviewproject_matrix * glm::vec4(0, 1, -1, 1);
    std::cout << "+1[Y] = {" << transformed_y_pos_one.x << " " << transformed_y_pos_one.y << " " << transformed_y_pos_one.z << "}\n";
    transformed_y_neg_one = modelviewproject_matrix * glm::vec4(0, -1, -1, 1);
    std::cout << "-1[Y] = {" << transformed_y_neg_one.x << " " << transformed_y_neg_one.y << " " << transformed_y_neg_one.z << "}\n";
    exit(0);
    */
    modelviewproject_matrix *= rotation_matrix;

    normal_matrix = glm::transpose(glm::inverse(glm::mat3(modelviewproject_matrix)));
    glBindVertexArray(vao);

    viewproject = modelviewproject_matrix;
    viewmatrix = modelviewproject_matrix;
    normalmatrix = normal_matrix;
    lightspacematrix = lightspace_matrix * rotation_matrix;
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
    humanoid->render_dag(false);

    error = glGetError();
    std::cout << "At the end, glError = " << error << ", " << glewGetErrorString(error) << "\n";
    //exit(0);
}

void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 
    
    std::cout << "---------------" << "\n";
    std::cout << "Debug message (" << id << "): " <<  message << "\n";
    fout << "---------------" << "\n";
    fout << "Debug message (" << id << "): " <<  message << "\n";

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << "\n";

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             fout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   fout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: fout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     fout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     fout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           fout << "Source: Other"; break;
    } fout << "\n";
    
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << "\n";

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               fout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: fout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  fout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         fout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         fout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              fout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          fout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           fout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               fout << "Type: Other"; break;
    } fout << "\n";
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << "\n";
    std::cout << "\n";

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         fout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       fout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          fout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: fout << "Severity: notification"; break;
    } fout << "\n";
    fout << "\n";
}

int main(int argc, char** argv) {
    fout.open("debug.log");
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
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    glDebugMessageCallback(glDebugOutput, nullptr);

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

