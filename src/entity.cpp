#include "entity.hpp"

AnimationEntity::AnimationEntity(std::string name, HierarchyNode *root) {
    this->name = name;
    this->params.clear();
    this->part_to_param_indices.clear();
    this->root = root;

    build_part_to_params(root);
}

void AnimationEntity::build_part_to_params(HierarchyNode *node) {
    int start, end;
    start = this->params.size();
    end = start + node->n_dof;
    for(int i = 0; i < node->n_dof; i++) {
        this->params.push_back(node->dof_params[i].first);
    }
    this->part_to_param_indices[node->name] = std::pair<int, int>(start, end);
    for(auto it : node->children) {
        build_part_to_params(it);
    }
}

void AnimationEntity::extract_params(HierarchyNode *node) {
    int start, end, i;
    auto range = this->part_to_param_indices[node->name];
    start = range.first;
    end = range.second;
    for(i = 0; i < node->n_dof; i++) {
        this->params[start + i] = node->dof_params[i].first;
    }
    assert(start + i == end);
    for(auto it : node->children) {
        extract_params(it);
    }
}

void AnimationEntity::apply_params(HierarchyNode *node) {
    int start, end, i;
    std::cout << "Applying params to " << node->name << "\n";
    auto range = this->part_to_param_indices[node->name];
    start = range.first;
    end = range.second;
    for(i = 0; i < node->n_dof; i++) {
        node->dof_params[i].first = this->params[start + i];
    }
    node->update_dof_transform();
    assert(start + i == end);
    for(auto it : node->children) {
        apply_params(it);
    }
}

void AnimationEntity::read_params_from_file(std::string fname) {
    std::ifstream in;
    in.open(fname);
    int n;
    if(in.is_open()) {
        in >> n;
        assert(this->params.size() == n);
        for(int i = 0; i < n; i++) {
            in >> this->params[i];
        }
        in.close();
        apply_params(this->root);
        this->root->update_dof_transform();
    }
    else {
        std::cout << "Unable to open file " << fname << "\n";
    }
}

void AnimationEntity::save_params_to_file(std::string fname) {
    extract_params(this->root);
    std::ofstream out;
    out.open(fname);
    if(out.is_open()) {
        out << this->params.size() << "\n";
        for(float p : this->params) {
            out << p << " ";
        }
        out << "\n";
        out.close();
    }
    else {
        std::cout << "Unable to open file " << fname << "\n";
    }
}
