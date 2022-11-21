#include "includes/gl_framework.hpp"
#include "includes/entity.hpp"
#include "includes/state.hpp"
#include "includes/camera.hpp"
#include <cmath>
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
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
            /* Mod keys to be put before normal keys for proper responses */
        } else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
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
        }  else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_Y) {
            xmove += moveamount;
        } else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_H) {
            xmove -= moveamount;
        } else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_J) {
            ymove += moveamount;
        } else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_G) {
            ymove -= moveamount;
        }else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_T) {
            zmove += moveamount;
        } else if(mods == GLFW_MOD_ALT && key == GLFW_KEY_U) {
            zmove -= moveamount;
        } else if(key == GLFW_KEY_J) {
            xrot += rotamount;
        } else if(key == GLFW_KEY_G) {
            xrot -= rotamount;
        } else if(key == GLFW_KEY_Y) {
            yrot += rotamount;
            if(yrot > M_PI/2 - 0.1) yrot = M_PI/2 - 0.1;
        } else if(key == GLFW_KEY_H) {
            yrot -= rotamount;
            if(yrot < -M_PI/2 + 0.1) yrot = -M_PI/2 + 0.1;
        }else if(key == GLFW_KEY_T) {
            zrot += rotamount;
        } else if(key == GLFW_KEY_U) {
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
        } else if(key == GLFW_KEY_F1 && action == GLFW_PRESS) {
            global_animate_state.lights_list[0].isActive = !global_animate_state.lights_list[0].isActive;
        } else if(key == GLFW_KEY_F2 && action == GLFW_PRESS) {
            global_animate_state.lights_list[1].isActive = !global_animate_state.lights_list[1].isActive;
        } else if(key == GLFW_KEY_F3 && action == GLFW_PRESS) {
            global_animate_state.lights_list[2].isActive = !global_animate_state.lights_list[2].isActive;
        } else if(key == GLFW_KEY_F4 && action == GLFW_PRESS) {
            global_animate_state.lights_list[3].isActive = !global_animate_state.lights_list[3].isActive;
        } else if(key == GLFW_KEY_S && action == GLFW_PRESS) {
            unsigned int keyframe_idx;
            std::cout << "Enter keyframe index:";
            std::cin >> keyframe_idx;
            global_animate_state.save_keyframe(keyframe_idx);
            global_animate_state.save_keyframes_to_file();
        } else if(key == GLFW_KEY_P && action == GLFW_PRESS) {
            global_animate_state.playback_mode = true;
            global_animate_state.record_mode = false;
            global_animate_state.interpolate_keyframes();
            global_animate_state.start_playback();
        } else if(key == GLFW_KEY_L && action == GLFW_PRESS) {
            global_animate_state.read_keyframes_from_file();
        } else if(key == GLFW_KEY_R && action == GLFW_PRESS) {
            global_animate_state.record_mode = !global_animate_state.record_mode;
            if(global_animate_state.record_mode) {
                global_animate_state.playback_mode = true;
                global_animate_state.interpolate_keyframes();
                global_animate_state.start_playback();
            }
        } else if(key == GLFW_KEY_7) {
            glm::vec3 norm = third_person_camera.n;
            global_animate_state.entity_list[0].root->dof_params[3].first -= norm.z * global_animate_state.entity_list[0].root->dof_deltas[3];
            global_animate_state.entity_list[0].root->dof_params[4].first -= norm.y * global_animate_state.entity_list[0].root->dof_deltas[4];
            global_animate_state.entity_list[0].root->dof_params[5].first -= norm.x * global_animate_state.entity_list[0].root->dof_deltas[5];
            global_animate_state.entity_list[0].root->update_dof_transform();
            global_animate_state.entity_list[1].root->dof_params[3].first -= norm.z * global_animate_state.entity_list[1].root->dof_deltas[3];
            global_animate_state.entity_list[1].root->dof_params[4].first -= norm.y * global_animate_state.entity_list[1].root->dof_deltas[4];
            global_animate_state.entity_list[1].root->dof_params[5].first -= norm.x * global_animate_state.entity_list[1].root->dof_deltas[5];
            global_animate_state.entity_list[1].root->update_dof_transform();
        }  else if(key == GLFW_KEY_8) {
            glm::vec3 norm = third_person_camera.n;
            global_animate_state.entity_list[0].root->dof_params[3].first += norm.z * global_animate_state.entity_list[0].root->dof_deltas[3];
            global_animate_state.entity_list[0].root->dof_params[4].first += norm.y * global_animate_state.entity_list[0].root->dof_deltas[4];
            global_animate_state.entity_list[0].root->dof_params[5].first += norm.x * global_animate_state.entity_list[0].root->dof_deltas[5];
            global_animate_state.entity_list[0].root->update_dof_transform();
            global_animate_state.entity_list[1].root->dof_params[3].first += norm.z * global_animate_state.entity_list[1].root->dof_deltas[3];
            global_animate_state.entity_list[1].root->dof_params[4].first += norm.y * global_animate_state.entity_list[1].root->dof_deltas[4];
            global_animate_state.entity_list[1].root->dof_params[5].first += norm.x * global_animate_state.entity_list[1].root->dof_deltas[5];
            global_animate_state.entity_list[1].root->update_dof_transform();
        } else if(key == GLFW_KEY_9) {
            HierarchyNode *bike, *rider;
            glm::mat4 bike_local_transform, bike_resultant_transform;
            int rot_idx = dof_id;
            float tan, theta, theta_x, theta_y, theta_z;
            rider = global_animate_state.entity_list[0].root;
            bike = global_animate_state.entity_list[1].root;
            
            bike_local_transform = glm::inverse(rider->dof_transform) * bike->dof_transform;

            rider->dof_params[rot_idx].first -= rider->dof_deltas[rot_idx];
            rider->update_dof_transform();

            bike_resultant_transform = rider->dof_transform * bike_local_transform;
            // MVP -- https://stackoverflow.com/a/15029416
            bike->dof_params[2].first = atan2(bike_resultant_transform[1][2], bike_resultant_transform[2][2]);
            bike->dof_params[1].first = atan2(-bike_resultant_transform[0][2], sqrt(bike_resultant_transform[1][2] * bike_resultant_transform[1][2] + bike_resultant_transform[2][2] * bike_resultant_transform[2][2]));
            bike->dof_params[0].first = atan2(bike_resultant_transform[0][1], bike_resultant_transform[0][0]);
            bike->dof_params[3].first = bike_resultant_transform[3][2];
            bike->dof_params[4].first = bike_resultant_transform[3][1];
            bike->dof_params[5].first = bike_resultant_transform[3][0];
            bike->update_dof_transform();
        } else if(key == GLFW_KEY_0) {
            HierarchyNode *bike, *rider;
            glm::mat4 bike_local_transform, bike_resultant_transform;
            int rot_idx = dof_id;
            float tan, theta, theta_x, theta_y, theta_z;
            rider = global_animate_state.entity_list[0].root;
            bike = global_animate_state.entity_list[1].root;
            
            bike_local_transform = glm::inverse(rider->dof_transform) * bike->dof_transform;

            rider->dof_params[rot_idx].first += rider->dof_deltas[rot_idx];
            rider->update_dof_transform();

            bike_resultant_transform = rider->dof_transform * bike_local_transform;
            // MVP -- https://stackoverflow.com/a/15029416
            theta = atan2(bike_resultant_transform[1][2], bike_resultant_transform[2][2]);
            std::cout << theta << " ";
            bike->dof_params[2].first = theta > 0 ? theta : theta + 2 * M_PI;
            theta = atan2(-bike_resultant_transform[0][2], sqrt(bike_resultant_transform[1][2] * bike_resultant_transform[1][2] + bike_resultant_transform[2][2] * bike_resultant_transform[2][2]));
            std::cout << theta << " ";
            bike->dof_params[1].first = theta > 0 ? theta : theta + 2 * M_PI;
            theta = atan2(bike_resultant_transform[0][1], bike_resultant_transform[0][0]);
            std::cout << theta << "\n";
            bike->dof_params[0].first = theta > 0 ? theta : theta + 2 * M_PI;
            bike->dof_params[3].first = bike_resultant_transform[3][2];
            bike->dof_params[4].first = bike_resultant_transform[3][1];
            bike->dof_params[5].first = bike_resultant_transform[3][0];
            bike->update_dof_transform();
        }
    }
}
