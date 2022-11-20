#include "gl_framework.hpp"
#include "entity.hpp"
#include "state.hpp"
#include "camera.hpp"
#include <GLFW/glfw3.h>
extern bool persp;
extern float xrot, yrot, zrot, rotamount, VIEW_PADDING, zoomamount, light_x, light_y, light_z, light_moveamount;
extern float xmove, ymove, zmove, moveamount;
extern HierarchyNode *curr_node;
extern int entity_idx;
extern AnimationState global_animate_state;
extern Camera third_person_camera;
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
        }  else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_W) {
            xmove += moveamount;
        } else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_S) {
            xmove -= moveamount;
        } else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_D) {
            ymove += moveamount;
        } else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_A) {
            ymove -= moveamount;
        }else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_Q) {
            zmove += moveamount;
        } else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_E) {
            zmove -= moveamount;
        } else if(key == GLFW_KEY_D) {
            xrot += rotamount;
        } else if(key == GLFW_KEY_A) {
            xrot -= rotamount;
        } else if(key == GLFW_KEY_W) {
            yrot += rotamount;
            if(yrot > M_PI/2 - 0.1) yrot = M_PI/2 - 0.1;
        } else if(key == GLFW_KEY_S) {
            yrot -= rotamount;
            if(yrot < -M_PI/2 + 0.1) yrot = -M_PI/2 + 0.1;
        }else if(key == GLFW_KEY_Q) {
            zrot += rotamount;
        } else if(key == GLFW_KEY_E) {
            zrot -= rotamount;
        } else if(key == GLFW_KEY_COMMA && action == GLFW_PRESS) {
            if(entity_idx == 0) {
                std::cout << "Already at the leftmost entity : " << global_animate_state.entity_list[entity_idx].name << "\n";
            }
            else {
                entity_idx -= 1;
                curr_node = global_animate_state.entity_list[entity_idx].root;
                std::cout << "Moved to " << global_animate_state.entity_list[entity_idx].name << "." << curr_node->name << "\n";
            }
        } else if(key == GLFW_KEY_PERIOD && action == GLFW_PRESS) {
            if(entity_idx == global_animate_state.entity_list.size() - 1) {
                std::cout << "Already at the rightmost entity : " << global_animate_state.entity_list[entity_idx].name << "\n";
            }
            else {
                entity_idx += 1;
                curr_node = global_animate_state.entity_list[entity_idx].root;
                std::cout << "Moved to " << global_animate_state.entity_list[entity_idx].name << "." << curr_node->name << "\n";
            }
        } else if(key == GLFW_KEY_K && action == GLFW_PRESS) {
            auto curr = global_animate_state.entity_list[entity_idx];
            curr.save_params_to_file("params_" + curr.name + ".txt");
        } else if(key == GLFW_KEY_B && action == GLFW_PRESS){
            global_animate_state.curr_camera = 0;
        } else if(key == GLFW_KEY_N && action == GLFW_PRESS){
            global_animate_state.curr_camera = 1;
        } else if(key == GLFW_KEY_M && action == GLFW_PRESS) {
            global_animate_state.curr_camera = 2;
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
        } else if(key == GLFW_KEY_F1 && action == GLFW_PRESS) {
            global_animate_state.lights_list[0].isActive = !global_animate_state.lights_list[0].isActive;
        } else if(key == GLFW_KEY_F2 && action == GLFW_PRESS) {
            global_animate_state.lights_list[1].isActive = !global_animate_state.lights_list[1].isActive;
        } else if(key == GLFW_KEY_F3 && action == GLFW_PRESS) {
            global_animate_state.lights_list[2].isActive = !global_animate_state.lights_list[2].isActive;
        } else if(key == GLFW_KEY_F4 && action == GLFW_PRESS) {
            global_animate_state.lights_list[3].isActive = !global_animate_state.lights_list[3].isActive;
        } else if(key == GLFW_KEY_F5 && action == GLFW_PRESS) {
            unsigned int keyframe_idx;
            std::cout << "Enter keyframe index:";
            std::cin >> keyframe_idx;
            global_animate_state.save_keyframe(keyframe_idx);
        } else if(key == GLFW_KEY_F6 && action == GLFW_PRESS) {
            global_animate_state.interpolate_keyframes();
        } else if(key == GLFW_KEY_F7 && action == GLFW_PRESS) {
            global_animate_state.start_playback();
        } else if(key == GLFW_KEY_F8 && action == GLFW_PRESS) {
            global_animate_state.save_keyframes_to_file();
        } else if(key == GLFW_KEY_F9 && action == GLFW_PRESS) {
            global_animate_state.read_keyframes_from_file();
        } else if(key == GLFW_KEY_F10 && action == GLFW_PRESS) {
            global_animate_state.record_mode = !global_animate_state.record_mode;
        } else if(key == GLFW_KEY_7 && action == GLFW_PRESS) {
            glm::vec3 norm = third_person_camera.n;
            global_animate_state.entity_list[0].root->dof_params[3].first += norm.z * global_animate_state.entity_list[0].root->dof_deltas[3];
            global_animate_state.entity_list[0].root->dof_params[4].first += norm.y * global_animate_state.entity_list[0].root->dof_deltas[4];
            global_animate_state.entity_list[0].root->dof_params[5].first += norm.x * global_animate_state.entity_list[0].root->dof_deltas[5];
            global_animate_state.entity_list[0].root->update_dof_transform();
            global_animate_state.entity_list[1].root->dof_params[3].first += norm.z * global_animate_state.entity_list[1].root->dof_deltas[3];
            global_animate_state.entity_list[1].root->dof_params[4].first += norm.y * global_animate_state.entity_list[1].root->dof_deltas[4];
            global_animate_state.entity_list[1].root->dof_params[5].first += norm.x * global_animate_state.entity_list[1].root->dof_deltas[5];
            global_animate_state.entity_list[1].root->update_dof_transform();
        } else if(key == GLFW_KEY_8 && action == GLFW_PRESS) {
            HierarchyNode *bike, *rider;
            glm::mat4 translate, translate_back, resultant;
            rider = global_animate_state.entity_list[0].root;
            bike = global_animate_state.entity_list[1].root;

            rider->dof_params[1].first += rider->dof_deltas[1];
            rider->update_dof_transform();

            bike->dof_params[1].first += bike->dof_deltas[1];
            bike->update_dof_transform();

            translate = glm::translate(glm::mat4(1), glm::vec3(
                rider->dof_params[5].first - bike->dof_params[5].first,
                rider->dof_params[4].first - bike->dof_params[4].first,
                rider->dof_params[3].first - bike->dof_params[3].first 
            ));

            translate_back = glm::translate(glm::mat4(1), glm::vec3(
                -rider->dof_params[5].first + bike->dof_params[5].first,
                -rider->dof_params[4].first + bike->dof_params[4].first,
                -rider->dof_params[3].first + bike->dof_params[3].first 
            ));

            resultant = translate * bike->dof_transform * translate_back;
            /*
            bike->dof_params[5].first = -rider->dof_params[5].first;
            bike->dof_params[4].first = -rider->dof_params[4].first;
            bike->dof_params[3].first = -rider->dof_params[3].first;
            */
            bike->update_dof_transform();
            bike->dof_transform = resultant;
            for(unsigned int i = 0; i < 4; i++) {
                for(unsigned int j = 0; j < 4; j++) {
                    std::cout << bike->dof_transform[i][j] << " ";
                }
                std::cout << "\n";
            }
            std::cout << bike->dof_params[3].first << " " << bike->dof_params[4].first << " " << bike->dof_params[5].first << "\n";
            for(unsigned int i = 0; i < 4; i++) {
                for(unsigned int j = 0; j < 4; j++) {
                    std::cout << resultant[i][j] << " ";
                }
                std::cout << "\n";
            }
            std::cout << "------------------------\n";
        }
    }
}
