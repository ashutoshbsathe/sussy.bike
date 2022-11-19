#ifndef __STATE_H__
#define __STATE_H__
#include "entity.hpp"
#include "light.hpp"
#include "camera.hpp"
typedef std::vector<float> Keyframe;
struct AnimationState {
    std::vector<AnimationEntity> entity_list;
    std::vector<Light> lights_list;
    Camera global_camera;
    int curr_camera = 0; // 0 -- global, 1 -- third person, 2 -- first person
    std::vector<Keyframe> saved_keyframes, interpolated_keyframes;
    Keyframe curr_keyframe;
    std::map<std::string, std::pair<unsigned int, unsigned int>> name_to_keyframe_indices;

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
        for(auto entity: this->entity_list) {
            start = this->name_to_keyframe_indices[entity.name].first;
            end = this->name_to_keyframe_indices[entity.name].second;
            for(int i = start; i < end; i++) {
                entity.params[i-start] = this->curr_keyframe[i];
            }
            entity.apply_params(entity.root);
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
};
#endif

