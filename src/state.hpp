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
};
#endif

