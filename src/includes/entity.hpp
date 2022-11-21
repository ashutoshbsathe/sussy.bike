#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_
#include "hnode.hpp"
#include "gl_framework.hpp"
#include <map>
#include <cassert>
#include <fstream>

struct AnimationEntity {
    std::string name;
    HierarchyNode *root;
    std::vector<float> params;
    std::map<std::string, std::pair<int, int>> part_to_param_indices;

    AnimationEntity(std::string name, HierarchyNode *root);
    
    void build_part_to_params(HierarchyNode *node);
    void extract_params(HierarchyNode *node);
    void apply_params(HierarchyNode *node);
    void save_params_to_file(std::string fname);
    void read_params_from_file(std::string fname);
};
#endif
