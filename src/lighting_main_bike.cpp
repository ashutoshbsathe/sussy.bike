#include "main.hpp"
#include "bike.hpp"
#include "rider.hpp"
#include "track.hpp"
#include <GLFW/glfw3.h>
#define MAX_BIKE_VBO_BYTES 1024000
//                          914112

double total_time = 0, rendered_frames = 0;
HierarchyNode *bike, *rider, *track, *curr_node;
int entity_idx = 0;

std::ofstream fout; // OpenGL logging

Camera third_person_camera, first_person_camera;

AnimationState global_animate_state;

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

void initGlobalAnimationState(void) {
    global_animate_state.global_camera = Camera(glm::vec3(0.f, 2000.f, -2000.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    global_animate_state.curr_camera = 0;
    global_animate_state.lights_list = {
        Light(
            glm::vec3(12500.f, 12500.f, 12500.f),
            glm::vec3(0.f, 0.f, 0.f),
            -10000,
            false
        ),
        Light(
            glm::vec3(-12500.f, 12500.f, 12500.f),
            glm::vec3(0.f, 0.f, 0.f),
            -10000,
            false
        ),
        Light(
            glm::vec3(0.f, 12500.f, 0.f),
            glm::vec3(0.f, 0.f, 0.f),
            cos(M_PI/36),
            false
        )
    };
}

void initShadersGL(void) {
    std::string vertex_shader_file("lighting_shading_vs.glsl");
    std::string fragment_shader_file("lighting_shading_fs.glsl");

    std::vector<GLuint> shaderList;
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

    shader_program = csX75::CreateProgramGL(shaderList);
    position_id = glGetAttribLocation(shader_program, "vPosition");
    color_id = glGetAttribLocation(shader_program, "vColor");
    normal_id = glGetAttribLocation(shader_program, "vNormal");
    uModelMatrix_id = glGetUniformLocation(shader_program, "uModelMatrix");
    uNormalMatrix_id = glGetUniformLocation(shader_program, "uNormalMatrix");
    uViewMatrix_id = glGetUniformLocation(shader_program, "uViewMatrix");
    uLightSpaceMatrix_id = glGetUniformLocation(shader_program, "uLightSpaceMatrix");
    uShadowMap_id = glGetUniformLocation(shader_program, "shadowMap");
    uNumLights_id = glGetUniformLocation(shader_program, "num_lights");
    uMaterial_id = glGetUniformLocation(shader_program, "material");
    
    shaderList.clear();
    shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, "shadow_mapping_vs.glsl"));
    shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, "shadow_mapping_fs.glsl"));

    shadow_shader_program = csX75::CreateProgramGL(shaderList);
    shadow_position_id = glGetAttribLocation(shadow_shader_program, "vPosition");
    shadow_uLightSpaceMatrix_id = glGetUniformLocation(shadow_shader_program, "uLightSpaceMatrix");

    initSkyboxShadersGL();
    initTexturedShadersGL();
}

void initVertexBufferGL(void) {
    unsigned int vbo_offset = 0, body_vbo_offset = 0;
    std::pair<HierarchyNode *, unsigned int> pair;
    std::map<std::string, GLuint> gl_info;

    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
 
    // TODO: See if this much memory is actually required
    glBufferData(GL_ARRAY_BUFFER, 3*MAX_BIKE_VBO_BYTES, NULL, GL_STATIC_DRAW);
    
    gl_info["uniform_xform_id"] = uModelMatrix_id;
    gl_info["normal_matrix_id"] = uNormalMatrix_id;
    gl_info["view_matrix_id"] = uViewMatrix_id;
    gl_info["light_space_matrix_id"] = uLightSpaceMatrix_id;
    gl_info["shadow_map_id"] = uShadowMap_id;
    gl_info["shadow_light_space_matrix_id"] = shadow_uLightSpaceMatrix_id;
    gl_info["vbo_offset"] = vbo_offset;
    pair = build_humanoid(gl_info);
    rider = pair.first;
    third_person_position = glm::vec3(0, 3000, 0);
    third_person_lookat = glm::vec3(0, 0, 0);
    first_person_position = glm::vec3(0, 0, 250);
    first_person_lookat_dir = glm::vec3(0, -1, 0);
    vbo_offset = pair.second;
    rider->prepare_vbo();
    global_animate_state.entity_list.push_back(AnimationEntity("standalone_rider", rider));
    global_animate_state.entity_list[0].read_params_from_file("./params_rider.txt");
    curr_node = rider;

    gl_info["uniform_xform_id"] = uModelMatrix_id;
    gl_info["normal_matrix_id"] = uNormalMatrix_id;
    gl_info["view_matrix_id"] = uViewMatrix_id;
    gl_info["light_space_matrix_id"] = uLightSpaceMatrix_id;
    gl_info["shadow_map_id"] = uShadowMap_id;
    gl_info["shadow_light_space_matrix_id"] = shadow_uLightSpaceMatrix_id;
    gl_info["vbo_offset"] = vbo_offset;
    pair = build_bike(gl_info);
    bike = pair.first;
    bike_headlight = ((
        bike->triangle_list[70].p1 +
        bike->triangle_list[70].p2 +
        bike->triangle_list[70].p3 +
        bike->triangle_list[71].p3
    ) * 0.25).to_vec3() + glm::vec3(150, 0, 0); // pull the headlight "out"
    bike_headlight_lookat_dir = glm::vec3(1, -1, 0);
    global_animate_state.lights_list.push_back(Light(bike_headlight, bike_headlight + bike_headlight_lookat_dir, cos(M_PI/36), false));
    vbo_offset = pair.second;
    bike->prepare_vbo();
    global_animate_state.entity_list.push_back(AnimationEntity("standalone_bike", bike));
    global_animate_state.entity_list[1].read_params_from_file("./params_bike.txt");
    curr_node = bike;
    
    gl_info["uniform_xform_id"] = uModelMatrix_id;
    gl_info["normal_matrix_id"] = uNormalMatrix_id;
    gl_info["view_matrix_id"] = uViewMatrix_id;
    gl_info["light_space_matrix_id"] = uLightSpaceMatrix_id;
    gl_info["shadow_map_id"] = uShadowMap_id;
    gl_info["shadow_light_space_matrix_id"] = shadow_uLightSpaceMatrix_id;
    gl_info["vbo_offset"] = vbo_offset;
    pair = build_track(gl_info);
    track = pair.first;
    vbo_offset = pair.second;
    track->prepare_vbo();
    global_animate_state.entity_list.push_back(AnimationEntity("standalone_track", track));
 
    std::cout << "VBO successfully initialized\n";
    // Enable the vertex attribute
    // Excellent answer -- https://stackoverflow.com/a/39684775
    glEnableVertexAttribArray (position_id);
    glVertexAttribPointer (position_id, 3, GL_FLOAT, GL_FALSE, 3 * 3 * sizeof(float), BUFFER_OFFSET(0));

    glEnableVertexAttribArray(color_id);
    glVertexAttribPointer(color_id, 3, GL_FLOAT, GL_FALSE, 3 * 3 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));

    glEnableVertexAttribArray(normal_id);
    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 3 * 3 * sizeof(float), BUFFER_OFFSET(3 * 2 * sizeof(float)));

    /* Init depth buffer for shadow mapping */
    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap_texture);
    glBindTexture(GL_TEXTURE_2D, depthMap_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMap_width, depthMap_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap_texture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &depthMap_texture_array);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap_texture_array);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, depthMap_width, depthMap_height, 4, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    global_animate_state.build_name_to_keyframe_indices();

    initSkyboxBuffersGL();
    initTexturedBuffersGL();
}

void renderScene(glm::mat4 viewproject, glm::mat4 view, std::vector<glm::mat4> lightspace, glm::mat4 rider_hierarchy, glm::mat4 bike_hierarchy, bool lightcam) {
    hnode_viewproject = viewproject;
    hnode_viewmatrix = view;
    hnode_lightspacematrix = lightspace;
    hnode_hierarchy_matrix_stack = rider_hierarchy;
    rider->render_dag(lightcam);

    hnode_viewproject = viewproject;
    hnode_viewmatrix = view;
    hnode_lightspacematrix = lightspace;
    hnode_hierarchy_matrix_stack = bike_hierarchy;
    bike->render_dag(lightcam);

    hnode_viewproject = viewproject;
    hnode_viewmatrix = view;
    hnode_lightspacematrix = lightspace;
    hnode_hierarchy_matrix_stack = glm::mat4(1);
    track->render_dag(lightcam);
}

void updateLightCameraParams(int light_idx) {
    if(light_idx >= global_animate_state.lights_list.size())
        return;
    int idx;
    switch(light_idx) {
        case 0: break;
        case 1: break;
        case 2:
            // follows humanoid
            global_animate_state.entity_list[0].extract_params(global_animate_state.entity_list[0].root);
            idx = global_animate_state.entity_list[0].part_to_param_indices[global_animate_state.entity_list[0].root->name].first;
            global_animate_state.lights_list[light_idx].spotPoint.z = global_animate_state.entity_list[0].params[idx+3];
            global_animate_state.lights_list[light_idx].spotPoint.y = global_animate_state.entity_list[0].params[idx+4];
            global_animate_state.lights_list[light_idx].spotPoint.x = global_animate_state.entity_list[0].params[idx+5];
            break;
        case 3: 
            glm::vec3 normal = glm::normalize(bike_headlight_lookat_dir);
            glm::mat4 model_handlebar = bike->dof_transform * bike->children[3]->dof_transform;
            glm::mat4 model_bike = bike->local_transform * bike->dof_transform * bike->private_transform * bike->children[3]->local_transform * bike->children[3]->dof_transform * bike->children[3]->private_transform;
            normal = glm::transpose(glm::inverse(glm::mat3(model_handlebar))) * normal;
            global_animate_state.lights_list[light_idx].position = glm::vec3(model_bike * glm::vec4(bike_headlight, 1));
            global_animate_state.lights_list[light_idx].spotPoint = global_animate_state.lights_list[light_idx].position + normal;
            break;
    }
}

void renderGL(void) { 
    glfwSetTime(0);
    glBindVertexArray(vao);
    // render into depthmap
    std::vector<glm::mat4> lightspace_matrices;
    lightspace_matrices.clear();
    for(unsigned int i = 0; i < global_animate_state.lights_list.size(); i++) {
        updateLightCameraParams(i);
        glViewport(0, 0, depthMap_width, depthMap_height);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view_matrix = global_animate_state.lights_list[i].to_camera().viewMatrix;
        projection_matrix = glm::ortho(-15000.f, 15000.f, -15000.f, 15000.f, 0.f, 50000.f);
        lightspace_matrix = projection_matrix * view_matrix;

        glUseProgram(shadow_shader_program);
        
        renderScene(lightspace_matrix, view_matrix, {lightspace_matrix}, glm::mat4(1), glm::mat4(1), true);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        lightspace_matrices.push_back(lightspace_matrix);
        glCopyImageSubData(depthMap_texture, GL_TEXTURE_2D, 0, 0, 0, 0, depthMap_texture_array, GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, depthMap_width, depthMap_height, 1);
    }
    // normal rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if(global_animate_state.curr_camera == 0) {
        global_animate_state.global_camera.eye = global_animate_state.global_camera.eye + xmove * global_animate_state.global_camera.n;
        global_animate_state.global_camera.eye = global_animate_state.global_camera.eye + ymove * global_animate_state.global_camera.u;
        global_animate_state.global_camera.eye = global_animate_state.global_camera.eye + zmove * global_animate_state.global_camera.v;
        xmove = ymove = zmove = 0;
        global_animate_state.global_camera.updateCameraVectors();
        global_animate_state.global_camera.yaw += xrot;
        global_animate_state.global_camera.pitch += yrot;
        xrot = yrot = 0;
        global_animate_state.global_camera.updateCameraVectors();

        view_matrix = global_animate_state.global_camera.viewMatrix;
        projection_matrix = glm::frustum(-1,1,-1,1,1,10);
    } else if(global_animate_state.curr_camera == 1) {
        third_person_camera.eye = glm::vec3(rider->local_transform * rider->dof_transform * rider->private_transform * glm::vec4(third_person_position, 1));
        third_person_camera.focusAtPoint(glm::vec3(rider->local_transform * rider->dof_transform * rider->private_transform * glm::vec4(third_person_lookat, 1)));
        third_person_camera.updateCameraVectors();

        view_matrix = third_person_camera.viewMatrix;
        projection_matrix = glm::frustum(-1,1,-1,1,1,10);
    } else if(global_animate_state.curr_camera == 2) {
        glm::mat4 model = HEAD_GLOBAL_TRANSFORM(rider);
        first_person_camera.eye = glm::vec3(model * glm::vec4(first_person_position, 1));
        glm::vec3 transformed_dir = glm::transpose(glm::inverse(glm::mat3(model))) * first_person_lookat_dir;
        auto spotPoint = first_person_camera.eye + transformed_dir;
        first_person_camera.focusAtPoint(spotPoint);

        first_person_camera.updateCameraVectors();

        view_matrix = first_person_camera.viewMatrix;
        projection_matrix = glm::frustum(-1, 1, -1, 1, 1, 10);
    }
    modelviewproject_matrix = projection_matrix * view_matrix; 
    
    renderSkyboxGL(modelviewproject_matrix);
    renderTexturedGL();

    glUseProgram(shader_program);
    glBindVertexArray(vao);
    glUniformMatrix4fv(uViewMatrix_id, 1, GL_FALSE, glm::value_ptr(modelviewproject_matrix)); 
    glUniform4f(uMaterial_id, 0.75, 0.5, 1.2, 2);
    glUniform1i(uShadowMap_id, 0);
    glUniform1i(uNumLights_id, global_animate_state.lights_list.size());
    push_lights_to_uniform(shader_program, global_animate_state.lights_list);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, depthMap_texture_array);      
    renderScene(modelviewproject_matrix, modelviewproject_matrix, lightspace_matrices, glm::mat4(1), glm::mat4(1), false);
    
    total_time += glfwGetTime();
    rendered_frames += 1;

    std::cout << "FPS: " << rendered_frames / total_time << "\n";
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
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; exit(0);break;
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    glDebugMessageCallback(glDebugOutput, nullptr);

    initGlobalAnimationState();
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

