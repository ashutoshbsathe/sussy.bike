#include "hnode.hpp"

glm::mat4 viewproject = glm::mat4(1);
glm::mat4 hierarchy_matrix_stack = glm::mat4(1);

HierarchyNode::HierarchyNode() {
    this->name = "";

    this->parent = NULL;
    this->children.clear();

    this->triangle_list.clear();
    this->line_list.clear();
    this->vbo_copy = NULL;
    this->local_transform = glm::mat4(1);
    this->private_transform = glm::mat4(1);

    this->n_dof = 0;
    this->init_default_dof_transform();
    this->dof_transform = glm::mat4(1);

    this->vao = 0;
    this->vbo = 0;
    this->vbo_offset = 0;
    this->uniform_xform_id = 0;

    this->draw_triangle = (this->triangle_list.size() > 0);
    this->draw_line = (this->line_list.size() > 0);
}

HierarchyNode::HierarchyNode(StackedPolyPrism p) {
    this->name = p.name;

    this->parent = NULL;
    this->children.clear();

    this->line_list.clear();
    this->triangle_list = p.triangle_list;
    this->vbo_copy = new float[p.triangle_list.size() * 3 * 3 * 2]();
    for(int i = 0; i < p.triangle_list.size(); i++) {
        this->vbo_copy[18*i] = p.triangle_list[i].p1.x;
        this->vbo_copy[18*i+1] = p.triangle_list[i].p1.y;
        this->vbo_copy[18*i+2] = p.triangle_list[i].p1.z;
        this->vbo_copy[18*i+3] = p.triangle_list[i].c.x;
        this->vbo_copy[18*i+4] = p.triangle_list[i].c.y;
        this->vbo_copy[18*i+5] = p.triangle_list[i].c.z;
        this->vbo_copy[18*i+6] = p.triangle_list[i].p2.x;
        this->vbo_copy[18*i+7] = p.triangle_list[i].p2.y;
        this->vbo_copy[18*i+8] = p.triangle_list[i].p2.z;
        this->vbo_copy[18*i+9] = p.triangle_list[i].c.x;
        this->vbo_copy[18*i+10] = p.triangle_list[i].c.y;
        this->vbo_copy[18*i+11] = p.triangle_list[i].c.z;
        this->vbo_copy[18*i+12] = p.triangle_list[i].p3.x;
        this->vbo_copy[18*i+13] = p.triangle_list[i].p3.y;
        this->vbo_copy[18*i+14] = p.triangle_list[i].p3.z;
        this->vbo_copy[18*i+15] = p.triangle_list[i].c.x;
        this->vbo_copy[18*i+16] = p.triangle_list[i].c.y;
        this->vbo_copy[18*i+17] = p.triangle_list[i].c.z;
    }
    this->local_transform = p.init_transform;
    this->private_transform = glm::mat4(1);
    
    this->init_default_dof_transform();
    this->dof_transform = glm::mat4(1);

    this->vao = 0;
    this->vbo = 0;
    this->vbo_offset = 0;
    this->uniform_xform_id = 0;

    this->draw_triangle = (this->triangle_list.size() > 0);
    this->draw_line = (this->line_list.size() > 0);
}

HierarchyNode::HierarchyNode(std::string fname) {
    std::ifstream in;
    std::string line;
    std::stringstream ss;
    std::vector<Point> points;
    float x, y, z, r, g, b;
    int n, t;
    in.open(fname);
    if(in.is_open()) {
        this->parent = NULL;
        this->children.clear();

        this->triangle_list.clear();
        this->line_list.clear();
        try {
            std::getline(in, line);
            ss = std::stringstream();ss << line;
            ss >> this->name;

            std::getline(in, line);
            ss = std::stringstream();ss << line;
            ss >> n >> t;

            for(int i = 0; i < n; i++) {
                std::getline(in, line);
                ss = std::stringstream();ss << line;
                ss >> x >> y >> z;
                points.push_back(Point(x, y, z));
            }
            
            for(int i = 0; i < t; i++) {
                std::getline(in, line);
                ss = std::stringstream();ss << line;
                ss >> x >> y >> z >> r >> g >> b;
                this->triangle_list.push_back(Triangle(
                    points[x], points[y], points[z],
                    Point(r, g, b)
                ));
            }
            assert(this->triangle_list.size() == t);
            /* this->vbo_copy needs to be created manually */
            this->vbo_copy = new float[t * 3 * 3 * 2]();
            for(int i = 0; i < this->triangle_list.size(); i++) {
                this->vbo_copy[18*i] = this->triangle_list[i].p1.x;
                this->vbo_copy[18*i+1] = this->triangle_list[i].p1.y;
                this->vbo_copy[18*i+2] = this->triangle_list[i].p1.z;
                this->vbo_copy[18*i+3] = this->triangle_list[i].c.x;
                this->vbo_copy[18*i+4] = this->triangle_list[i].c.y;
                this->vbo_copy[18*i+5] = this->triangle_list[i].c.z;
                this->vbo_copy[18*i+6] = this->triangle_list[i].p2.x;
                this->vbo_copy[18*i+7] = this->triangle_list[i].p2.y;
                this->vbo_copy[18*i+8] = this->triangle_list[i].p2.z;
                this->vbo_copy[18*i+9] = this->triangle_list[i].c.x;
                this->vbo_copy[18*i+10] = this->triangle_list[i].c.y;
                this->vbo_copy[18*i+11] = this->triangle_list[i].c.z;
                this->vbo_copy[18*i+12] = this->triangle_list[i].p3.x;
                this->vbo_copy[18*i+13] = this->triangle_list[i].p3.y;
                this->vbo_copy[18*i+14] = this->triangle_list[i].p3.z;
                this->vbo_copy[18*i+15] = this->triangle_list[i].c.x;
                this->vbo_copy[18*i+16] = this->triangle_list[i].c.y;
                this->vbo_copy[18*i+17] = this->triangle_list[i].c.z; 
            }
            this->local_transform = glm::mat4(1);
            this->private_transform = glm::mat4(1);

            this->init_default_dof_transform();
            this->dof_transform = glm::mat4(1);

            this->vao = 0;
            this->vbo = 0;
            this->vbo_offset = 0;
            this->uniform_xform_id = 0;

            this->draw_triangle = (this->triangle_list.size() > 0);
            this->draw_line = (this->line_list.size() > 0);
        }
        catch(std::exception const &e) {
            std::cout << "error : " << e.what() << "\n";
        }
    }
    else {
        std::cout << "Unable to open file " << fname << "\n";
    }
}

HierarchyNode::HierarchyNode(std::string name, std::vector<Triangle> t, std::vector<Triangle> l) {
    this->name = name;

    this->parent = NULL;
    this->children.clear();

    this->triangle_list.clear();
    this->line_list.clear();

    this->triangle_list.insert(this->triangle_list.end(), t.begin(), t.end());
    this->line_list.insert(this->line_list.end(), l.begin(), l.end());

    this->vbo_copy = new float[t.size() * 3 * 3 * 2 + l.size() * 2 * 3 * 2]();
    for(int i = 0; i < this->triangle_list.size(); i++) {
        this->vbo_copy[18*i] = this->triangle_list[i].p1.x;
        this->vbo_copy[18*i+1] = this->triangle_list[i].p1.y;
        this->vbo_copy[18*i+2] = this->triangle_list[i].p1.z;
        this->vbo_copy[18*i+3] = this->triangle_list[i].c.x;
        this->vbo_copy[18*i+4] = this->triangle_list[i].c.y;
        this->vbo_copy[18*i+5] = this->triangle_list[i].c.z;
        this->vbo_copy[18*i+6] = this->triangle_list[i].p2.x;
        this->vbo_copy[18*i+7] = this->triangle_list[i].p2.y;
        this->vbo_copy[18*i+8] = this->triangle_list[i].p2.z;
        this->vbo_copy[18*i+9] = this->triangle_list[i].c.x;
        this->vbo_copy[18*i+10] = this->triangle_list[i].c.y;
        this->vbo_copy[18*i+11] = this->triangle_list[i].c.z;
        this->vbo_copy[18*i+12] = this->triangle_list[i].p3.x;
        this->vbo_copy[18*i+13] = this->triangle_list[i].p3.y;
        this->vbo_copy[18*i+14] = this->triangle_list[i].p3.z;
        this->vbo_copy[18*i+15] = this->triangle_list[i].c.x;
        this->vbo_copy[18*i+16] = this->triangle_list[i].c.y;
        this->vbo_copy[18*i+17] = this->triangle_list[i].c.z; 
    }
    for(int i = this->triangle_list.size(), j = 0; j < this->line_list.size(); i++, j++) {
        this->vbo_copy[12*i] = this->line_list[j].p1.x;
        this->vbo_copy[12*i+1] = this->line_list[j].p1.y;
        this->vbo_copy[12*i+2] = this->line_list[j].p1.z;
        this->vbo_copy[12*i+3] = this->line_list[j].c.x;
        this->vbo_copy[12*i+4] = this->line_list[j].c.y;
        this->vbo_copy[12*i+5] = this->line_list[j].c.z;
        this->vbo_copy[12*i+6] = this->line_list[j].p2.x;
        this->vbo_copy[12*i+7] = this->line_list[j].p2.y;
        this->vbo_copy[12*i+8] = this->line_list[j].p2.z;
        this->vbo_copy[12*i+9] = this->line_list[j].c.x;
        this->vbo_copy[12*i+10] = this->line_list[j].c.y;
        this->vbo_copy[12*i+11] = this->line_list[j].c.z;
    }
    this->local_transform = glm::mat4(1);
    this->private_transform = glm::mat4(1);

    this->init_default_dof_transform();
    this->dof_transform = glm::mat4(1);

    this->vao = 0;
    this->vbo = 0;
    this->vbo_offset = 0;
    this->uniform_xform_id = 0;

    this->draw_triangle = (this->triangle_list.size() > 0);
    this->draw_line = (this->line_list.size() > 0);
}

HierarchyNode::~HierarchyNode() {
    for(auto it : this->children) {
        delete it;
    }
    delete this->vbo_copy;
    delete this;
}

void HierarchyNode::make_rigid() {
    this->n_dof = 0;
    this->dof_deltas = {0, 0, 0, 0, 0, 0};
    this->dof_limits = {
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
    };
    this->dof_params = {
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
}

void HierarchyNode::init_default_dof_transform() {
    this->n_dof = 6;
    this->dof_deltas = {M_PI/12., M_PI/12., M_PI/12., 5, 5, 5};
    this->dof_limits = {
        {-M_PI, M_PI},
        {-M_PI, M_PI},
        {-M_PI, M_PI},
        {-100000, 100000},
        {-100000, 100000},
        {-100000, 100000},
    };
    this->dof_params = {
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
}

void HierarchyNode::update_dof_transform() {
    this->dof_transform = glm::mat4(1);
    this->dof_transform = glm::rotate(this->dof_transform, this->dof_params[0].first, this->dof_params[0].second);
    this->dof_transform = glm::rotate(this->dof_transform, this->dof_params[1].first, this->dof_params[1].second);
    this->dof_transform = glm::rotate(this->dof_transform, this->dof_params[2].first, this->dof_params[2].second);
    this->dof_transform = glm::translate(this->dof_transform, this->dof_params[3].first * this->dof_params[3].second);
    this->dof_transform = glm::translate(this->dof_transform, this->dof_params[4].first * this->dof_params[4].second);
    this->dof_transform = glm::translate(this->dof_transform, this->dof_params[5].first * this->dof_params[5].second);
    for(auto it : this->children) {
        it->update_dof_transform();
    }
}

void HierarchyNode::prepare_vbo() {
    glBufferSubData(GL_ARRAY_BUFFER, this->vbo_offset * 3 * 2 * sizeof(float), this->triangle_list.size() * 3 * 3 * 2 * sizeof(float) + this->line_list.size() * 2 * 3 * 2 * sizeof(float), this->vbo_copy);
    for(auto it : this->children) {
        it->prepare_vbo();
    }
}

void HierarchyNode::render() {
    glm::mat4 overall = viewproject * hierarchy_matrix_stack * this->private_transform;
    glUniformMatrix4fv(this->uniform_xform_id, 1, GL_FALSE, glm::value_ptr(overall)); // value_ptr needed for proper pointer conversion
    if(draw_triangle) {
        glDrawArrays(GL_TRIANGLES, this->vbo_offset, this->triangle_list.size() * 3);
    }
    if(draw_line) {
        glDrawArrays(GL_LINES, this->vbo_offset + this->triangle_list.size() * 3, this->line_list.size() * 2);
    }
}

void HierarchyNode::render_dag() {
    glm::mat4 old_hierarchy_matrix_stack = glm::mat4(1.0f);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            old_hierarchy_matrix_stack[i][j] = hierarchy_matrix_stack[i][j];
        }
    }
    hierarchy_matrix_stack = hierarchy_matrix_stack * this->local_transform * this->dof_transform;
    render();
    for(auto it : this->children) {
        it->render_dag();
    }
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            hierarchy_matrix_stack[i][j] = old_hierarchy_matrix_stack[i][j];
        }
    }
}

void global_to_local(HierarchyNode *n) {
    // Assumes current local_transform is actually a global transform
    glm::mat4 old_hierarchy_matrix_stack = hierarchy_matrix_stack, old_global = n->local_transform;
    std::cout << "At " << n->name << "\n";
    std::cout << "Before\n";
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << n->local_transform[i][j] << " ";
        }
    }
    std::cout << "\n";
    n->local_transform = glm::inverse(hierarchy_matrix_stack) * n->local_transform;
    std::cout << "After\n";
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << n->local_transform[i][j] << " ";
        }
    }

    auto new_node = StackedPolyPrism("./bike_parts/" + n->name + ".txt");
    new_node.init_transform = n->local_transform;
    std::ofstream fout;
    fout.open("./bike_parts/" + n->name + ".txt");
    fout << new_node.to_str();
    fout.close();

    std::cout << "\n---------------------------------------------------------\n";
    hierarchy_matrix_stack = old_global; // much more numerically stable
    for(auto it : n->children) {
        global_to_local(it);
    }
    hierarchy_matrix_stack = old_hierarchy_matrix_stack;
}

void add_edge(HierarchyNode *parent, HierarchyNode *child, unsigned int *next_available_vbo_offset) {
    // Add an edge from parent to child
    // Adjust GL related properties first
    child->vbo_offset = *next_available_vbo_offset;
    child->vbo = parent->vbo;
    child->vao = parent->vao;
    child->uniform_xform_id = parent->uniform_xform_id;
    *next_available_vbo_offset += 3 * child->triangle_list.size() + 2 * child->line_list.size();
    parent->children.push_back(child);
    child->parent = parent;
}
