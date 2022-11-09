#include "gl_framework.hpp"
#include "entity.hpp"
#include <GLFW/glfw3.h>
extern bool persp;
extern float xrot, yrot, zrot, rotamount, VIEW_PADDING, zoomamount, light_x, light_y, light_z, light_moveamount;
extern HierarchyNode *curr_node;
extern std::vector<AnimationEntity> entities;
extern int entity_idx;
extern bool lightcam;
namespace csX75 {

    unsigned int dof_id = 0;
    void update_dof_id(unsigned int new_dof_id) {
        if(new_dof_id >= curr_node->n_dof + 1 || new_dof_id == 0) {
            std::cout << "dof#" << new_dof_id << " does not exist for " << curr_node->name << "\n";
        }
        dof_id = new_dof_id - 1;
        auto params = curr_node->dof_params[dof_id];
        std::cout << curr_node->name << "[dof#" << dof_id + 1 << "]: value = " << params.first << ", delta = " << curr_node->dof_deltas[dof_id] << ", along (" << params.second[0] << ", " << params.second[1] << ", " << params.second[2] << ")\n";
    }

    //! Initialize GL State
    void initGL(void) {
        //Set framebuffer clear color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //Set depth buffer furthest depth
        glClearDepth(1.0);
        //Set depth test to less-than
        glDepthFunc(GL_LESS);
        //Enable depth testing
        glEnable(GL_DEPTH_TEST);
        //Enable Gourard shading
        glShadeModel(GL_SMOOTH);
    }

    //!GLFW Error Callback
    void error_callback(int error, const char* description) {
        std::cerr<<description<<std::endl;
    }

    //!GLFW framebuffer resize callback
    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        //!Resize the viewport to fit the window size - draw to entire window
        glViewport(0, 0, width, height);
    }

    //!GLFW keyboard callback
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        //!Close the window if the ESC key was pressed
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        /* Mod keys to be put before */
        else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
            if(curr_node->parent == NULL) {
                std::cout << "No sibling to " << curr_node->name << "\n";
            }
            else {
                std::vector<std::string> siblings;
                for(auto it : curr_node->parent->children) {
                    siblings.push_back(it->name);
                }
                int idx; // some weirdness in C++, resort to manual for loop
                for(idx = 0; idx < siblings.size(); idx++) {
                    if(siblings[idx] == curr_node->name)
                        break;
                }
                if(idx == 0) {
                    std::cout << curr_node->name << " is the leftmost sibling\n";
                }
                else {
                    curr_node = curr_node->parent->children[idx-1];
                    std::cout << "Moved to " << curr_node->name << "\n";
                }
            }
        } else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_RIGHT && action == GLFW_PRESS) { 
            if(curr_node->parent == NULL) {
                std::cout << "No sibling to " << curr_node->name << "\n";
            }
            else {
                std::vector<std::string> siblings;
                for(auto it : curr_node->parent->children) {
                    siblings.push_back(it->name);
                }
                int idx; // some weirdness in C++, resort to manual for loop
                for(idx = 0; idx < siblings.size(); idx++) {
                    if(siblings[idx] == curr_node->name)
                        break;
                }
                if(idx + 1 >= siblings.size()) {
                    std::cout << curr_node->name << " is the rightmost sibling\n";
                }
                else {
                    curr_node = curr_node->parent->children[idx+1];
                    std::cout << "Moved to " << curr_node->name << "\n";
                }
            }
        } else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_UP && action == GLFW_PRESS) {
            if(curr_node->parent == NULL) {
                std::cout << "No parent to " << curr_node->name << "\n";
            }
            else {
                curr_node = curr_node->parent; 
                std::cout << "Moved to " << curr_node->name << "\n";
            }
        } else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
            if(curr_node->children.size() == 0) {
                std::cout << "No children to " << curr_node->name << "\n";
            }
            else {
                curr_node = curr_node->children[0];
                std::cout << "Moved to " << curr_node->name << "\n";
            }
        } else if(key == GLFW_KEY_1 && action == GLFW_PRESS) {
            update_dof_id(1);
        } else if(key == GLFW_KEY_2 && action == GLFW_PRESS) {
            update_dof_id(2);
        } else if(key == GLFW_KEY_3 && action == GLFW_PRESS) {
            update_dof_id(3);
        } else if(key == GLFW_KEY_4 && action == GLFW_PRESS) {
            update_dof_id(4);
        } else if(key == GLFW_KEY_5 && action == GLFW_PRESS) {
            update_dof_id(5);
        } else if(key == GLFW_KEY_6 && action == GLFW_PRESS) {
            update_dof_id(6);
        } else if(key == GLFW_KEY_RIGHT_BRACKET) {
            if(curr_node->dof_params[dof_id].first + curr_node->dof_deltas[dof_id] <= curr_node->dof_limits[dof_id].second) {
                curr_node->dof_params[dof_id].first += curr_node->dof_deltas[dof_id];
            }
            curr_node->update_dof_transform();
            auto params = curr_node->dof_params[dof_id];
            std::cout << curr_node->name << "[dof#" << dof_id + 1 << "]: value = " << params.first << ", delta = " << curr_node->dof_deltas[dof_id] << ", along (" << params.second[0] << ", " << params.second[1] << ", " << params.second[2] << ")\n";
        } else if(key == GLFW_KEY_LEFT_BRACKET){
            if(curr_node->dof_params[dof_id].first - curr_node->dof_deltas[dof_id] >= curr_node->dof_limits[dof_id].first) {
                curr_node->dof_params[dof_id].first -= curr_node->dof_deltas[dof_id];
            }
            curr_node->update_dof_transform();
            auto params = curr_node->dof_params[dof_id];
            std::cout << curr_node->name << "[dof#" << dof_id + 1 << "]: value = " << params.first << ", delta = " << curr_node->dof_deltas[dof_id] << ", along (" << params.second[0] << ", " << params.second[1] << ", " << params.second[2] << ")\n";
        } else if(key == GLFW_KEY_W) {
            xrot += rotamount;
        } else if(key == GLFW_KEY_S) {
            xrot -= rotamount;
        } else if(key == GLFW_KEY_A) {
            yrot += rotamount;
        } else if(key == GLFW_KEY_D) {
            yrot -= rotamount;
        }else if(key == GLFW_KEY_Q) {
            zrot += rotamount;
        } else if(key == GLFW_KEY_E) {
            zrot -= rotamount;
        } else if(key == GLFW_KEY_COMMA && action == GLFW_PRESS) {
            if(entity_idx == 0) {
                std::cout << "Already at the leftmost entity : " << entities[entity_idx].name << "\n";
            }
            else {
                entity_idx -= 1;
                curr_node = entities[entity_idx].root;
                std::cout << "Moved to " << entities[entity_idx].name << "." << curr_node->name << "\n";
            }
        } else if(key == GLFW_KEY_PERIOD && action == GLFW_PRESS) {
            if(entity_idx == entities.size() - 1) {
                std::cout << "Already at the rightmost entity : " << entities[entity_idx].name << "\n";
            }
            else {
                entity_idx += 1;
                curr_node = entities[entity_idx].root;
                std::cout << "Moved to " << entities[entity_idx].name << "." << curr_node->name << "\n";
            }
        } else if(key == GLFW_KEY_K && action == GLFW_PRESS) {
            auto curr = entities[entity_idx];
            curr.save_params_to_file("params_" + curr.name + ".txt");
        } else if(key == GLFW_KEY_M && action == GLFW_PRESS){
            VIEW_PADDING += zoomamount;
        } else if(key == GLFW_KEY_N && action == GLFW_PRESS){
            VIEW_PADDING -= zoomamount;
        } else if(key == GLFW_KEY_Z && action == GLFW_PRESS) {
            lightcam = !lightcam;
        } else if(key == GLFW_KEY_UP) {
            light_y += light_moveamount;
        } else if(key == GLFW_KEY_DOWN) {
            light_y -= light_moveamount;
        }  else if(key == GLFW_KEY_RIGHT) {
            light_x += light_moveamount;
        } else if(key == GLFW_KEY_LEFT) {
            light_x -= light_moveamount;
        }  else if(key == GLFW_KEY_PAGE_UP) {
            light_z += light_moveamount;
        } else if(key == GLFW_KEY_PAGE_DOWN) {
            light_z -= light_moveamount;
        } 
    }
}
