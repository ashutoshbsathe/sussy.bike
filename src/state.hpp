#ifndef __STATE_H__
#define __STATE_H__
#include "entity.hpp"
#include "light.hpp"
#include "camera.hpp"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/matrix.hpp"
#include "glm/common.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
typedef std::vector<float> Keyframe;
struct AnimationState {
    std::vector<AnimationEntity> entity_list;
    std::vector<Light> lights_list;
    Camera global_camera;
    int curr_camera = 0; // 0 -- global, 1 -- third person, 2 -- first person
    std::vector<Keyframe> saved_keyframes, interpolated_keyframes;
    Keyframe curr_keyframe, restore_keyframe;
    std::map<std::string, std::pair<unsigned int, unsigned int>> name_to_keyframe_indices;

    bool playback_mode = false, record_mode = false;
    unsigned int playback_idx = 0;

    AnimationState() {
        this->entity_list.clear();
        this->lights_list.clear();
        this->global_camera = Camera();
        this->curr_camera = 0;
        this->saved_keyframes.clear();
        this->interpolated_keyframes.clear();
    }

    void build_name_to_keyframe_indices() {
        this->name_to_keyframe_indices.clear();
        this->curr_keyframe.clear();
        // keyframe index -- nth keyframe 
        this->curr_keyframe.push_back(0); // illegal value initially
        this->name_to_keyframe_indices["keyframe_idx"] = {0, 1};
        // lights -- each index corr to index in `lights_list`
        unsigned int start = curr_keyframe.size(), end = start;
        for(auto light: this->lights_list) {
            this->curr_keyframe.push_back(light.isActive);
            end++;
        }
        this->name_to_keyframe_indices["lights"] = {start, end};
        
        start = this->curr_keyframe.size();
        this->curr_keyframe.push_back(this->curr_camera);
        end = this->curr_keyframe.size();
        this->name_to_keyframe_indices["curr_camera"] = {start, end};

        start = this->curr_keyframe.size();
        curr_keyframe.push_back(this->global_camera.eye.x);
        curr_keyframe.push_back(this->global_camera.eye.y);
        curr_keyframe.push_back(this->global_camera.eye.z);
        curr_keyframe.push_back(this->global_camera.yaw);
        curr_keyframe.push_back(this->global_camera.pitch);
        end = this->curr_keyframe.size();
        this->name_to_keyframe_indices["global_camera"] = {start, end};

        for(auto entity: this->entity_list) {
            start = this->curr_keyframe.size();
            for(auto param: entity.params) {
                this->curr_keyframe.push_back(param);
            }
            end = this->curr_keyframe.size();
            this->name_to_keyframe_indices[entity.name] = {start, end};
        }

        for(auto it: this->name_to_keyframe_indices) {
            std::cout << it.first << ": [" << it.second.first << ", " << it.second.second << "]\n";
        }
    }

    void extract_keyframe() {
        unsigned int start, end;
        // lights
        start = this->name_to_keyframe_indices["lights"].first;
        end = this->name_to_keyframe_indices["lights"].second;
        for(int i = start; i < end; i++) {
            this->curr_keyframe[i] = this->lights_list[i-start].isActive;
        }
        
        // current camera
        start = this->name_to_keyframe_indices["curr_camera"].first;
        this->curr_keyframe[start] = this->curr_camera; 
        
        // global camera
        start = this->name_to_keyframe_indices["global_camera"].first;
        this->curr_keyframe[start] = this->global_camera.eye.x;
        this->curr_keyframe[start+1] = this->global_camera.eye.y;
        this->curr_keyframe[start+2] = this->global_camera.eye.z;
        this->curr_keyframe[start+3] = this->global_camera.yaw;
        this->curr_keyframe[start+4] = this->global_camera.pitch;

        // entities
        for(auto entity: this->entity_list) {
            start = this->name_to_keyframe_indices[entity.name].first;
            end = this->name_to_keyframe_indices[entity.name].second;
            entity.extract_params(entity.root);
            for(int i = start; i < end; i++) {
                this->curr_keyframe[i] = entity.params[i-start];
            }
        }

        for(float p: this->curr_keyframe) {
            std::cout << p << " ";
        }
        std::cout << "\n";
    }
    
    void apply_keyframe() {
        unsigned int start, end;
        // lights
        start = this->name_to_keyframe_indices["lights"].first;
        end = this->name_to_keyframe_indices["lights"].second;
        for(int i = start; i < end; i++) {
            this->lights_list[i-start].isActive = this->curr_keyframe[i];
        }
        
        // current camera
        start = this->name_to_keyframe_indices["curr_camera"].first;
        this->curr_camera = this->curr_keyframe[start]; 
        
        // global camera
        start = this->name_to_keyframe_indices["global_camera"].first;
        this->global_camera.eye.x = this->curr_keyframe[start];
        this->global_camera.eye.y = this->curr_keyframe[start+1];
        this->global_camera.eye.z = this->curr_keyframe[start+2];
        this->global_camera.yaw = this->curr_keyframe[start+3];
        this->global_camera.pitch = this->curr_keyframe[start+4];

        // entities
        for(unsigned int i = 0; i < this->entity_list.size(); i++) {
            start = this->name_to_keyframe_indices[this->entity_list[i].name].first;
            end = this->name_to_keyframe_indices[this->entity_list[i].name].second;
            for(unsigned int j = start; j < end; j++) {
                this->entity_list[i].params[j-start] = this->curr_keyframe[j];
            }
            this->entity_list[i].apply_params(this->entity_list[i].root);
        }
    }

    void save_keyframe(unsigned int keyframe_idx) {
        this->extract_keyframe();
        this->curr_keyframe[0] = keyframe_idx;
        this->saved_keyframes.push_back(Keyframe(this->curr_keyframe.begin(), this->curr_keyframe.end()));
        int i = 0;
        for(auto keyframe: this->saved_keyframes) {
            std::cout << "i = " << i << ", [ ";
            for(auto param: keyframe) {
                std::cout << param << " ";
            }
            std::cout << "]\n";
            i++;
        }
    }

    void interpolate_keyframes() {
        this->interpolated_keyframes.clear();
        unsigned int n_saved = this->saved_keyframes.size(), start, end;
        std::cout << "n_saved = " << n_saved << "\n";
        if(n_saved < 2) {
            return;
        }
        this->interpolated_keyframes.push_back(Keyframe(this->saved_keyframes[0].begin(), this->saved_keyframes[0].end()));
        for(unsigned int i = 1; i < n_saved; i++) {
            // src is already pushed, tgt will be pushed at last
            Keyframe src = this->saved_keyframes[i-1], tgt = this->saved_keyframes[i];
            for(unsigned int j = src[0]+1; j < tgt[0]; j++) {
                Keyframe k_i = Keyframe(src.begin(), src.end()); 
                k_i[0] = j;
                // lights
                start = this->name_to_keyframe_indices["lights"].first;
                end = this->name_to_keyframe_indices["lights"].second;
                for(int k = start; k < end; k++) {
                    k_i[k] = src[k];
                }
                
                // current camera
                start = this->name_to_keyframe_indices["curr_camera"].first;
                k_i[start] = src[start];
                
                // global camera
                start = this->name_to_keyframe_indices["global_camera"].first;
                k_i[start] = src[start] + ((tgt[start] - src[start]) / (tgt[0] - src[0])) * (j - src[0]);
                k_i[start+1] = src[start+1] + ((tgt[start+1] - src[start+1]) / (tgt[0] - src[0])) * (j - src[0]);
                k_i[start+2] = src[start+2] + ((tgt[start+2] - src[start+2]) / (tgt[0] - src[0])) * (j - src[0]);
                k_i[start+3] = src[start+3] + ((tgt[start+3] - src[start+3]) / (tgt[0] - src[0])) * (j - src[0]);
                k_i[start+4] = src[start+4] + ((tgt[start+4] - src[start+4]) / (tgt[0] - src[0])) * (j - src[0]);

                // entities
                for(auto entity: this->entity_list) {
                    start = this->name_to_keyframe_indices[entity.name].first;
                    end = this->name_to_keyframe_indices[entity.name].second;
                    for(int k = start; k < end; k++) {
                        k_i[k] = src[k] + ((tgt[k] - src[k]) / (tgt[0] - src[0])) * (j - src[0]);
                    }
                }

                // hardcoded euler angle -> quaternion SLERP -> euler for bike and body
                /*
                std::cout << src[54] << " " << src[55] << " " << src[56] << "\n";
                std::cout << tgt[54] << " " << tgt[55] << " " << tgt[56] << "\n";
                std::cout << this->curr_keyframe[54] << " " << this->curr_keyframe[55] << " " << this->curr_keyframe[56] << "\n";
                */
                glm::mat4 bike_rot_src = glm::rotate(glm::mat4(1),src[54], this->entity_list[1].root->dof_params[0].second);
                bike_rot_src = glm::rotate(bike_rot_src, src[55], this->entity_list[1].root->dof_params[1].second);
                bike_rot_src = glm::rotate(bike_rot_src, src[56], this->entity_list[1].root->dof_params[2].second);
                glm::mat4 bike_rot_tgt = glm::rotate(glm::mat4(1),tgt[54], this->entity_list[1].root->dof_params[0].second);
                bike_rot_tgt = glm::rotate(bike_rot_tgt, tgt[55], this->entity_list[1].root->dof_params[1].second);
                bike_rot_tgt = glm::rotate(bike_rot_tgt, tgt[56], this->entity_list[1].root->dof_params[2].second);

                glm::quat src_q = glm::quat_cast(bike_rot_src), tgt_q = glm::quat_cast(bike_rot_tgt), j_q;
                j_q = glm::mix(src_q, tgt_q, (j*1.f - src[0]) / (tgt[0] - src[0]));
                glm::mat3 resultant_matrix = glm::mat3_cast(j_q);
                
                this->curr_keyframe[56] = atan2(resultant_matrix[1][2], resultant_matrix[2][2]);
                this->curr_keyframe[55] = atan2(-resultant_matrix[0][2], sqrt(resultant_matrix[1][2] * resultant_matrix[1][2] + resultant_matrix[2][2] * resultant_matrix[2][2]));
                this->curr_keyframe[54] = atan2(resultant_matrix[0][1], resultant_matrix[0][0]);

                /*
                std::cout << this->curr_keyframe[54] << " " << this->curr_keyframe[55] << " " << this->curr_keyframe[56] << "\n";
                std::cout << "-----------------------\n";
                exit(0);
                */
                this->interpolated_keyframes.push_back(Keyframe(k_i.begin(), k_i.end()));
            }
            this->interpolated_keyframes.push_back(Keyframe(tgt.begin(), tgt.end()));
        }
        int i = 0;
        for(auto keyframe: this->interpolated_keyframes) {
            std::cout << "i = " << i << ", [ ";
            for(auto param: keyframe) {
                std::cout << param << " ";
            }
            std::cout << "]\n";
            i++;
        }
    }

    void start_playback(void) {
        // save the original keyframe for restoring later
        this->curr_keyframe[0] = 0;
        this->extract_keyframe();
        this->restore_keyframe = Keyframe(this->curr_keyframe.begin(), this->curr_keyframe.end());
        this->playback_idx = 0;
        this->playback_mode = true;
        glfwSetTime(0);
    }

    void stop_playback(void) {
        // restore the original keyframe
        this->curr_keyframe = this->restore_keyframe;
        this->apply_keyframe();
        this->playback_idx = 0;
        this->playback_mode = false;
    }

    void save_keyframes_to_file(void) {
        std::ofstream fout;
        fout.open("keyframes.txt");
        int num_keyframes = 0, frames = 0;
        for(Keyframe f: this->saved_keyframes) {
            for(float param: f) {
                fout << param << " ";
            }
            fout << "\n";
            num_keyframes++;
            frames = f[0];
        }
        std::cout << "Wrote " << num_keyframes << " keyframes worth " << frames << " frames\n";
        fout.close();
    }

    void read_keyframes_from_file(void) {
        std::ifstream fin;
        fin.open("keyframes.txt");
        std::string line;
        std::stringstream ss;
        int num_keyframes = 0;
        while(std::getline(fin, line)) {
            ss.str("");
            ss.str(line);
            std::cout << ss.str() << ", was read\n";
            std::cout << line << ", curr_keyframe.size() = " << this->curr_keyframe.size() << "\n";
            
            for(int i = 0; i < this->curr_keyframe.size(); i++) {
                ss >> this->curr_keyframe[i];
                std::cout << this->curr_keyframe[i] << ",";
            }
            std::cout << "\n";
            this->saved_keyframes.push_back(Keyframe(this->curr_keyframe.begin(), this->curr_keyframe.end()));
            num_keyframes++;
            std::cout << this->curr_keyframe[0] << "\n";
        }
        std::cout << "Read " << num_keyframes << " keyframes worth " << this->curr_keyframe[0] << " frames\n";
        fin.close();
    }
};
#endif

