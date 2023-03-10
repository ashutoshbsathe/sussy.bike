#include "includes/hnode.hpp"

glm::mat4 hnode_viewproject = glm::mat4(1);
glm::mat4 hnode_viewmatrix = glm::mat4(1);
std::vector<glm::mat4> hnode_lightspacematrix;
glm::mat4 hnode_hierarchy_matrix_stack = glm::mat4(1);

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

    this->draw_triangle = (this->triangle_list.size() > 0);
    this->draw_line = (this->line_list.size() > 0);
}

HierarchyNode::HierarchyNode(StackedPolyPrism p) {
    this->name = p.name;

    this->parent = NULL;
    this->children.clear();

    this->line_list.clear();
    this->triangle_list = p.triangle_list;
    this->vbo_copy = new float[p.triangle_list.size() * 3 * 3 * 3]();
    for(int i = 0; i < p.triangle_list.size(); i++) {
        this->vbo_copy[27*i] = this->triangle_list[i].p1.x;
        this->vbo_copy[27*i+1] = this->triangle_list[i].p1.y;
        this->vbo_copy[27*i+2] = this->triangle_list[i].p1.z;
        this->vbo_copy[27*i+3] = this->triangle_list[i].c.x;
        this->vbo_copy[27*i+4] = this->triangle_list[i].c.y;
        this->vbo_copy[27*i+5] = this->triangle_list[i].c.z;
        this->vbo_copy[27*i+6] = this->triangle_list[i].p1.normal.x;
        this->vbo_copy[27*i+7] = this->triangle_list[i].p1.normal.y;
        this->vbo_copy[27*i+8] = this->triangle_list[i].p1.normal.z;
        this->vbo_copy[27*i+9] = this->triangle_list[i].p2.x;
        this->vbo_copy[27*i+10] = this->triangle_list[i].p2.y;
        this->vbo_copy[27*i+11] = this->triangle_list[i].p2.z;
        this->vbo_copy[27*i+12] = this->triangle_list[i].c.x;
        this->vbo_copy[27*i+13] = this->triangle_list[i].c.y;
        this->vbo_copy[27*i+14] = this->triangle_list[i].c.z;
        this->vbo_copy[27*i+15] = this->triangle_list[i].p2.normal.x;
        this->vbo_copy[27*i+16] = this->triangle_list[i].p2.normal.y;
        this->vbo_copy[27*i+17] = this->triangle_list[i].p2.normal.z;
        this->vbo_copy[27*i+18] = this->triangle_list[i].p3.x;
        this->vbo_copy[27*i+19] = this->triangle_list[i].p3.y;
        this->vbo_copy[27*i+20] = this->triangle_list[i].p3.z;
        this->vbo_copy[27*i+21] = this->triangle_list[i].c.x;
        this->vbo_copy[27*i+22] = this->triangle_list[i].c.y;
        this->vbo_copy[27*i+23] = this->triangle_list[i].c.z;
        this->vbo_copy[27*i+24] = this->triangle_list[i].p3.normal.x;
        this->vbo_copy[27*i+25] = this->triangle_list[i].p3.normal.y;
        this->vbo_copy[27*i+26] = this->triangle_list[i].p3.normal.z; 
        /*
        std::cout << "p1: " << this->triangle_list[i].p1.to_str() << "\n";
        std::cout << "p2: " << this->triangle_list[i].p2.to_str() << "\n";
        std::cout << "p3: " << this->triangle_list[i].p3.to_str() << "\n";
        */
    }
    this->local_transform = p.init_transform;
    this->private_transform = glm::mat4(1);
    
    this->init_default_dof_transform();
    this->dof_transform = glm::mat4(1);

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

            /* Generate vertex normals */
            int num_tri = this->triangle_list.size();
            std::map<Point, std::vector<glm::vec3>> point_to_all_normals;
            std::map<Point, glm::vec3> point_to_final_normal;

            for(auto t: this->triangle_list) {
                point_to_all_normals[t.p1].push_back(t.normal);
                point_to_all_normals[t.p2].push_back(t.normal);
                point_to_all_normals[t.p3].push_back(t.normal);
            }

            for(auto it: point_to_all_normals) {
                glm::vec3 tmp = glm::vec3(0,0,0);
                for(auto n: it.second) {
                    tmp += n;
                }
                tmp /= it.second.size();
                point_to_final_normal[it.first] = glm::normalize(tmp);
            }

            for(int i = 0; i < num_tri; i++) {
                this->triangle_list[i].p1.normal = point_to_final_normal[this->triangle_list[i].p1];
                this->triangle_list[i].p2.normal = point_to_final_normal[this->triangle_list[i].p2];
                this->triangle_list[i].p3.normal = point_to_final_normal[this->triangle_list[i].p3];
                /* 
                std::cout << "p1:" << this->triangle_list[i].p1.to_str() << "\n";
                std::cout << "p2:" << this->triangle_list[i].p2.to_str() << "\n";
                std::cout << "p3:" << this->triangle_list[i].p3.to_str() << "\n";
                */
            }
            /* this->vbo_copy needs to be created manually */
            this->vbo_copy = new float[t * 3 * 3 * 3]();
            for(int i = 0; i < this->triangle_list.size(); i++) {
                this->vbo_copy[27*i] = this->triangle_list[i].p1.x;
                this->vbo_copy[27*i+1] = this->triangle_list[i].p1.y;
                this->vbo_copy[27*i+2] = this->triangle_list[i].p1.z;
                this->vbo_copy[27*i+3] = this->triangle_list[i].c.x;
                this->vbo_copy[27*i+4] = this->triangle_list[i].c.y;
                this->vbo_copy[27*i+5] = this->triangle_list[i].c.z;
                this->vbo_copy[27*i+6] = this->triangle_list[i].p1.normal.x;
                this->vbo_copy[27*i+7] = this->triangle_list[i].p1.normal.y;
                this->vbo_copy[27*i+8] = this->triangle_list[i].p1.normal.z;
                this->vbo_copy[27*i+9] = this->triangle_list[i].p2.x;
                this->vbo_copy[27*i+10] = this->triangle_list[i].p2.y;
                this->vbo_copy[27*i+11] = this->triangle_list[i].p2.z;
                this->vbo_copy[27*i+12] = this->triangle_list[i].c.x;
                this->vbo_copy[27*i+13] = this->triangle_list[i].c.y;
                this->vbo_copy[27*i+14] = this->triangle_list[i].c.z;
                this->vbo_copy[27*i+15] = this->triangle_list[i].p2.normal.x;
                this->vbo_copy[27*i+16] = this->triangle_list[i].p2.normal.y;
                this->vbo_copy[27*i+17] = this->triangle_list[i].p2.normal.z;
                this->vbo_copy[27*i+18] = this->triangle_list[i].p3.x;
                this->vbo_copy[27*i+19] = this->triangle_list[i].p3.y;
                this->vbo_copy[27*i+20] = this->triangle_list[i].p3.z;
                this->vbo_copy[27*i+21] = this->triangle_list[i].c.x;
                this->vbo_copy[27*i+22] = this->triangle_list[i].c.y;
                this->vbo_copy[27*i+23] = this->triangle_list[i].c.z;
                this->vbo_copy[27*i+24] = this->triangle_list[i].p3.normal.x;
                this->vbo_copy[27*i+25] = this->triangle_list[i].p3.normal.y;
                this->vbo_copy[27*i+26] = this->triangle_list[i].p3.normal.z; 
            }
            this->local_transform = glm::mat4(1);
            this->private_transform = glm::mat4(1);

            this->init_default_dof_transform();
            this->dof_transform = glm::mat4(1);

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

    /* Generate vertex normals */
    int num_tri = this->triangle_list.size();
    std::map<Point, std::vector<glm::vec3>> point_to_all_normals;
    std::map<Point, glm::vec3> point_to_final_normal;

    for(auto t: this->triangle_list) {
        point_to_all_normals[t.p1].push_back(t.normal);
        point_to_all_normals[t.p2].push_back(t.normal);
        point_to_all_normals[t.p3].push_back(t.normal);
    }

    for(auto it: point_to_all_normals) {
        glm::vec3 tmp = glm::vec3(0,0,0);
        for(auto n: it.second) {
            tmp += n;
        }
        tmp /= it.second.size();
        point_to_final_normal[it.first] = glm::normalize(tmp);
    }

    for(int i = 0; i < num_tri; i++) {
        this->triangle_list[i].p1.normal = point_to_final_normal[this->triangle_list[i].p1];
        this->triangle_list[i].p2.normal = point_to_final_normal[this->triangle_list[i].p2];
        this->triangle_list[i].p3.normal = point_to_final_normal[this->triangle_list[i].p3];
       
        /*
        std::cout << "p1:" << this->triangle_list[i].p1.to_str() << "\n";
        std::cout << "p2:" << this->triangle_list[i].p2.to_str() << "\n";
        std::cout << "p3:" << this->triangle_list[i].p3.to_str() << "\n";
        */
    }
    this->vbo_copy = new float[t.size() * 3 * 3 * 3 + l.size() * 2 * 3 * 3]();
    for(int i = 0; i < this->triangle_list.size(); i++) {
        this->vbo_copy[27*i] = this->triangle_list[i].p1.x;
        this->vbo_copy[27*i+1] = this->triangle_list[i].p1.y;
        this->vbo_copy[27*i+2] = this->triangle_list[i].p1.z;
        this->vbo_copy[27*i+3] = this->triangle_list[i].c.x;
        this->vbo_copy[27*i+4] = this->triangle_list[i].c.y;
        this->vbo_copy[27*i+5] = this->triangle_list[i].c.z;
        this->vbo_copy[27*i+6] = this->triangle_list[i].p1.normal.x;
        this->vbo_copy[27*i+7] = this->triangle_list[i].p1.normal.y;
        this->vbo_copy[27*i+8] = this->triangle_list[i].p1.normal.z;
        this->vbo_copy[27*i+9] = this->triangle_list[i].p2.x;
        this->vbo_copy[27*i+10] = this->triangle_list[i].p2.y;
        this->vbo_copy[27*i+11] = this->triangle_list[i].p2.z;
        this->vbo_copy[27*i+12] = this->triangle_list[i].c.x;
        this->vbo_copy[27*i+13] = this->triangle_list[i].c.y;
        this->vbo_copy[27*i+14] = this->triangle_list[i].c.z;
        this->vbo_copy[27*i+15] = this->triangle_list[i].p2.normal.x;
        this->vbo_copy[27*i+16] = this->triangle_list[i].p2.normal.y;
        this->vbo_copy[27*i+17] = this->triangle_list[i].p2.normal.z;
        this->vbo_copy[27*i+18] = this->triangle_list[i].p3.x;
        this->vbo_copy[27*i+19] = this->triangle_list[i].p3.y;
        this->vbo_copy[27*i+20] = this->triangle_list[i].p3.z;
        this->vbo_copy[27*i+21] = this->triangle_list[i].c.x;
        this->vbo_copy[27*i+22] = this->triangle_list[i].c.y;
        this->vbo_copy[27*i+23] = this->triangle_list[i].c.z;
        this->vbo_copy[27*i+24] = this->triangle_list[i].p3.normal.x;
        this->vbo_copy[27*i+25] = this->triangle_list[i].p3.normal.y;
        this->vbo_copy[27*i+26] = this->triangle_list[i].p3.normal.z; 
    }
    for(int j = 0; j < this->line_list.size(); j++) {
        this->vbo_copy[27*num_tri+18*j] = this->line_list[j].p1.x;
        this->vbo_copy[27*num_tri+18*j+1] = this->line_list[j].p1.y;
        this->vbo_copy[27*num_tri+18*j+2] = this->line_list[j].p1.z;
        this->vbo_copy[27*num_tri+18*j+3] = this->line_list[j].c.x;
        this->vbo_copy[27*num_tri+18*j+4] = this->line_list[j].c.y;
        this->vbo_copy[27*num_tri+18*j+5] = this->line_list[j].c.z;
        this->vbo_copy[27*num_tri+18*j+6] = this->line_list[j].p1.normal.x;
        this->vbo_copy[27*num_tri+18*j+7] = this->line_list[j].p1.normal.y;
        this->vbo_copy[27*num_tri+18*j+8] = this->line_list[j].p1.normal.z;
        this->vbo_copy[27*num_tri+18*j+9] = this->line_list[j].p2.x;
        this->vbo_copy[27*num_tri+18*j+10] = this->line_list[j].p2.y;
        this->vbo_copy[27*num_tri+18*j+11] = this->line_list[j].p2.z;
        this->vbo_copy[27*num_tri+18*j+12] = this->line_list[j].c.x;
        this->vbo_copy[27*num_tri+18*j+13] = this->line_list[j].c.y;
        this->vbo_copy[27*num_tri+18*j+14] = this->line_list[j].c.z;
        this->vbo_copy[27*num_tri+18*j+15] = this->line_list[j].p2.normal.x;
        this->vbo_copy[27*num_tri+18*j+16] = this->line_list[j].p2.normal.y;
        this->vbo_copy[27*num_tri+18*j+17] = this->line_list[j].p2.normal.z;
    }
    this->local_transform = glm::mat4(1);
    this->private_transform = glm::mat4(1);

    this->init_default_dof_transform();
    this->dof_transform = glm::mat4(1);

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

void HierarchyNode::setGLInfo(std::map<std::string, GLuint> gl_info) {
    for(auto it : gl_info) {
        this->gl_info[it.first] = it.second;
    }
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
    this->dof_deltas = {M_PI/72., M_PI/72., M_PI/72., 50, 50, 50};
    this->dof_limits = {
        {0, 2*M_PI},
        {0, 2*M_PI},
        {0, 2*M_PI},
        {-100000, 100000},
        {-100000, 100000},
        {-100000, 100000},
    };
    this->dof_params = {
        /* Yes, it was fairly retarded to go z,x,y order */
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
    if(this->parent == NULL) {
        auto translation_to_origin = glm::translate(this->dof_transform, -this->dof_params[3].first * this->dof_params[3].second);
        translation_to_origin = glm::translate(translation_to_origin, -this->dof_params[4].first * this->dof_params[4].second);
        translation_to_origin = glm::translate(translation_to_origin, -this->dof_params[5].first * this->dof_params[5].second);
        auto rotation = glm::rotate(glm::mat4(1), this->dof_params[0].first, this->dof_params[0].second);
        rotation = glm::rotate(rotation, this->dof_params[1].first, this->dof_params[1].second);
        rotation = glm::rotate(rotation, this->dof_params[2].first, this->dof_params[2].second);
        auto translation_back = glm::translate(this->dof_transform, this->dof_params[3].first * this->dof_params[3].second);
        translation_back = glm::translate(translation_back, this->dof_params[4].first * this->dof_params[4].second);
        translation_back = glm::translate(translation_back, this->dof_params[5].first * this->dof_params[5].second);
        this->dof_transform = translation_back * rotation * translation_to_origin;
        this->dof_transform = glm::translate(this->dof_transform, this->dof_params[3].first * this->dof_params[3].second);
        this->dof_transform = glm::translate(this->dof_transform, this->dof_params[4].first * this->dof_params[4].second);
        this->dof_transform = glm::translate(this->dof_transform, this->dof_params[5].first * this->dof_params[5].second);
    }
    else {
        this->dof_transform = glm::rotate(this->dof_transform, this->dof_params[0].first, this->dof_params[0].second);
        this->dof_transform = glm::rotate(this->dof_transform, this->dof_params[1].first, this->dof_params[1].second);
        this->dof_transform = glm::rotate(this->dof_transform, this->dof_params[2].first, this->dof_params[2].second);
        this->dof_transform = glm::translate(this->dof_transform, this->dof_params[3].first * this->dof_params[3].second);
        this->dof_transform = glm::translate(this->dof_transform, this->dof_params[4].first * this->dof_params[4].second);
        this->dof_transform = glm::translate(this->dof_transform, this->dof_params[5].first * this->dof_params[5].second);
    }
    for(auto it : this->children) {
        it->update_dof_transform();
    }
}

void HierarchyNode::prepare_vbo() {
    glBufferSubData(GL_ARRAY_BUFFER, this->gl_info["vbo_offset"] * 3 * 3 * sizeof(float), this->triangle_list.size() * 3 * 3 * 3 * sizeof(float) + this->line_list.size() * 2 * 3 * 3 * sizeof(float), this->vbo_copy);
    for(auto it : this->children) {
        it->prepare_vbo();
    }
}

void HierarchyNode::render(bool shadowmap) {
    glm::mat4 overall_model = hnode_hierarchy_matrix_stack * this->private_transform;
    glm::mat4 overall = hnode_viewproject * overall_model;
    glm::mat3 overall_normals = glm::transpose(glm::inverse(glm::mat3(overall_model)));
    GLint error = glGetError();
    if(shadowmap) {
        glUniformMatrix4fv(this->gl_info["shadow_light_space_matrix_id"], 1, GL_FALSE, glm::value_ptr(overall)); // WHERE HAVE YOU INITIALIZED THIS 'shadow_light_space_matrix_id' ?
    }
    else {
        unsigned int num_lights = hnode_lightspacematrix.size();
        std::vector<glm::mat4> overall_lightspace;
        float copy_to_uniform_lightspace[4 * 16]; // Max 4 mat4s
        for(unsigned int i = 0; i < num_lights; i++) {
            overall_lightspace.push_back(hnode_lightspacematrix[i] * hnode_hierarchy_matrix_stack * this->private_transform);
        }
        for(unsigned int i = 0; i < num_lights; i++) 
            for(unsigned int j = 0; j < 4; j++)
                for(unsigned int k = 0; k < 4; k++)
                    copy_to_uniform_lightspace[i*16+j*4+k] = overall_lightspace[i][j][k];
        
        glUniformMatrix4fv(this->gl_info["uniform_xform_id"], 1, GL_FALSE, glm::value_ptr(overall_model)); // value_ptr needed for proper pointer conversion
        glUniformMatrix3fv(this->gl_info["normal_matrix_id"], 1, GL_FALSE, glm::value_ptr(overall_normals)); // value_ptr needed for proper pointer conversion
        glUniformMatrix4fv(this->gl_info["light_space_matrix_id"], num_lights, GL_FALSE, copy_to_uniform_lightspace); // value_ptr needed for proper pointer conversion
    }
    if(draw_triangle) {
        glDrawArrays(GL_TRIANGLES, this->gl_info["vbo_offset"], this->triangle_list.size() * 3);
    }
    if(draw_line) {
        glDrawArrays(GL_LINES, this->gl_info["vbo_offset"] + this->triangle_list.size() * 3, this->line_list.size() * 2);
    }
}

void HierarchyNode::render_dag(bool shadowmap) {
    // bool: shadowmap -- are we rendering for the shadowmap ?
    glm::mat4 old_hnode_hierarchy_matrix_stack = glm::mat4(1.0f);
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            old_hnode_hierarchy_matrix_stack[i][j] = hnode_hierarchy_matrix_stack[i][j];
        }
    }
    hnode_hierarchy_matrix_stack = hnode_hierarchy_matrix_stack * this->local_transform * this->dof_transform;
    render(shadowmap);
    for(auto it : this->children) {
        it->render_dag(shadowmap);
    }
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            hnode_hierarchy_matrix_stack[i][j] = old_hnode_hierarchy_matrix_stack[i][j];
        }
    }
}

void global_to_local(HierarchyNode *n) {
    // Assumes current local_transform is actually a global transform
    glm::mat4 old_hnode_hierarchy_matrix_stack = hnode_hierarchy_matrix_stack, old_global = n->local_transform;
    std::cout << "At " << n->name << "\n";
    std::cout << "Before\n";
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << n->local_transform[i][j] << " ";
        }
    }
    std::cout << "\n";
    n->local_transform = glm::inverse(hnode_hierarchy_matrix_stack) * n->local_transform;
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
    hnode_hierarchy_matrix_stack = old_global; // much more numerically stable
    for(auto it : n->children) {
        global_to_local(it);
    }
    hnode_hierarchy_matrix_stack = old_hnode_hierarchy_matrix_stack;
}

void add_edge(HierarchyNode *parent, HierarchyNode *child, unsigned int *next_available_vbo_offset) {
    // Add an edge from parent to child
    // Adjust GL related properties first
    child->setGLInfo(parent->gl_info);
    child->gl_info["vbo_offset"] = *next_available_vbo_offset;
    *next_available_vbo_offset += 3 * child->triangle_list.size() + 2 * child->line_list.size();
    parent->children.push_back(child);
    child->parent = parent;
}
