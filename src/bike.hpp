#include "hnode.hpp"

HierarchyNode *build_bike(GLuint vao, GLuint vbo, GLuint uniform_xform_id, GLuint normal_matrix_id, GLuint view_matrix_id, GLuint light_space_matrix_id, GLuint shadow_map_id, GLuint shadow_light_space_matrix_id, GLuint shadow_model_matrix_id) {
    unsigned int next_available_vbo_offset;
    StackedPolyPrism p_back_wheel_part_1 = StackedPolyPrism("./bike_parts/back_wheel_part_1.txt");
	HierarchyNode *back_wheel_part_1 = new HierarchyNode(p_back_wheel_part_1);
	StackedPolyPrism p_back_wheel_part_2 = StackedPolyPrism("./bike_parts/back_wheel_part_2.txt");
	HierarchyNode *back_wheel_part_2 = new HierarchyNode(p_back_wheel_part_2);
	StackedPolyPrism p_back_wheel_part_3 = StackedPolyPrism("./bike_parts/back_wheel_part_3.txt");
	HierarchyNode *back_wheel_part_3 = new HierarchyNode(p_back_wheel_part_3);
    StackedPolyPrism p_front_wheel_part_1 = StackedPolyPrism("./bike_parts/front_wheel_part_1.txt");
	HierarchyNode *front_wheel_part_1 = new HierarchyNode(p_front_wheel_part_1);
	StackedPolyPrism p_front_wheel_part_2 = StackedPolyPrism("./bike_parts/front_wheel_part_2.txt");
	HierarchyNode *front_wheel_part_2 = new HierarchyNode(p_front_wheel_part_2);
	StackedPolyPrism p_front_wheel_part_3 = StackedPolyPrism("./bike_parts/front_wheel_part_3.txt");
	HierarchyNode *front_wheel_part_3 = new HierarchyNode(p_front_wheel_part_3);
    HierarchyNode *handlebar = new HierarchyNode(StackedPolyPrism("./bike_parts/handlebar.txt"));
    HierarchyNode *footrest = new HierarchyNode(StackedPolyPrism("./bike_parts/footrest.txt"));
    HierarchyNode *engine = new HierarchyNode(StackedPolyPrism("./bike_parts/engine.txt"));
    HierarchyNode *front_scope_1 = new HierarchyNode(StackedPolyPrism("./bike_parts/front_scope_1.txt"));
    HierarchyNode *front_scope_2 = new HierarchyNode(StackedPolyPrism("./bike_parts/front_scope_2.txt"));
    HierarchyNode *body = new HierarchyNode("./bike_parts/body.txt");

    std::cout << "Initialized all bike parts\n";

    HierarchyNode *back_wheel_spokes, *front_wheel_spokes;
    std::vector<Triangle> spokes_list;
    spokes_list.clear();
    for(int i = 0; i < p_back_wheel_part_3.points[2].size(); i+=4) {
        int idx_next = (i+2) % p_back_wheel_part_2.points[1].size();
        Point rim_point = (p_back_wheel_part_2.points[1][idx_next] + p_back_wheel_part_2.points[2][idx_next]) * 0.5;

        spokes_list.push_back(Triangle(p_back_wheel_part_3.points[2][i], rim_point, Point(0, 0, 0), Point(1, 1, 1)));

        idx_next = (i+3) % p_back_wheel_part_2.points[1].size();
        rim_point = (p_back_wheel_part_2.points[1][idx_next] + p_back_wheel_part_2.points[2][idx_next]) * 0.5;

        spokes_list.push_back(Triangle(p_back_wheel_part_3.points[2][i+1], rim_point, Point(0, 0, 0), Point(1, 1, 1)));
    }
    for(int i = 2; i < p_front_wheel_part_3.points[3].size(); i+=4) {
        int idx_next = (i-2) % p_front_wheel_part_2.points[1].size();
        Point rim_point = (p_front_wheel_part_2.points[1][idx_next] + p_front_wheel_part_2.points[2][idx_next]) * 0.5;

        spokes_list.push_back(Triangle(p_back_wheel_part_3.points[2][i], rim_point, Point(0, 0, 0), Point(1, 1, 1)));

        idx_next = (i-1) % p_front_wheel_part_2.points[1].size();
        rim_point = (p_front_wheel_part_2.points[1][idx_next] + p_front_wheel_part_2.points[2][idx_next]) * 0.5;

        spokes_list.push_back(Triangle(p_back_wheel_part_3.points[2][i+1], rim_point, Point(0, 0, 0), Point(1, 1, 1)));
    }
    
    back_wheel_spokes = new HierarchyNode("back_wheel_spokes", {}, spokes_list);

    spokes_list.clear();
    for(int i = 0; i < p_front_wheel_part_3.points[2].size(); i+=4) {
        int idx_next = (i+2) % p_front_wheel_part_2.points[1].size();
        Point rim_point = (p_front_wheel_part_2.points[1][idx_next] + p_front_wheel_part_2.points[2][idx_next]) * 0.5;

        spokes_list.push_back(Triangle(p_front_wheel_part_3.points[2][i], rim_point, Point(0, 0, 0), Point(1, 1, 1)));

        idx_next = (i+3) % p_front_wheel_part_2.points[1].size();
        rim_point = (p_front_wheel_part_2.points[1][idx_next] + p_front_wheel_part_2.points[2][idx_next]) * 0.5;

        spokes_list.push_back(Triangle(p_front_wheel_part_3.points[2][i+1], rim_point, Point(0, 0, 0), Point(1, 1, 1)));
    }
    for(int i = 2; i < p_front_wheel_part_3.points[3].size(); i+=4) {
        int idx_next = (i-2) % p_front_wheel_part_2.points[1].size();
        Point rim_point = (p_front_wheel_part_2.points[1][idx_next] + p_front_wheel_part_2.points[2][idx_next]) * 0.5;

        spokes_list.push_back(Triangle(p_front_wheel_part_3.points[2][i], rim_point, Point(0, 0, 0), Point(1, 1, 1)));

        idx_next = (i-1) % p_front_wheel_part_2.points[1].size();
        rim_point = (p_front_wheel_part_2.points[1][idx_next] + p_front_wheel_part_2.points[2][idx_next]) * 0.5;

        spokes_list.push_back(Triangle(p_front_wheel_part_3.points[2][i+1], rim_point, Point(0, 0, 0), Point(1, 1, 1)));
    }
    
    front_wheel_spokes = new HierarchyNode("front_wheel_spokes", {}, spokes_list);

    front_scope_1->make_rigid();
    front_scope_2->make_rigid();
    back_wheel_part_2->make_rigid();
    back_wheel_part_3->make_rigid();
    back_wheel_spokes->make_rigid();
    front_wheel_part_2->make_rigid();
    front_wheel_part_3->make_rigid();
    front_wheel_spokes->make_rigid();
    engine->make_rigid();
    footrest->make_rigid();

    front_wheel_part_1->n_dof = 1;
    front_wheel_part_1->dof_limits = {
        {-2*M_PI, 2*M_PI},
        {0, 0},
        {0, 0},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    front_wheel_part_1->dof_deltas = {M_PI/36, 0, 0, 0, 0, 0};
    front_wheel_part_1->dof_params = {
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };

    back_wheel_part_1->n_dof = 1;
    back_wheel_part_1->dof_limits = {
        {-2*M_PI, 2*M_PI},
        {0, 0},
        {0, 0},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    back_wheel_part_1->dof_deltas = {M_PI/36, 0, 0, 0, 0, 0};
    back_wheel_part_1->dof_params = {
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
    
    handlebar->n_dof = 1;
    handlebar->dof_limits = {
        {-M_PI/15, 2*M_PI/15},
        {0, 0},
        {0, 0},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    handlebar->dof_deltas = {M_PI/36, 0, 0, 0, 0, 0};
    handlebar->dof_params = {
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };

    body->vao = vao;
    body->vbo = vbo;
    body->vbo_offset = 0;
    body->uniform_xform_id = uniform_xform_id;
    body->normal_matrix_id = normal_matrix_id;
    body->view_matrix_id = view_matrix_id;
    body->light_space_matrix_id = light_space_matrix_id;
    body->shadow_map_id = shadow_map_id;
    body->shadow_light_space_matrix_id = shadow_light_space_matrix_id;
    body->shadow_model_matrix_id = shadow_model_matrix_id;

    next_available_vbo_offset = 3 * body->triangle_list.size() + 2 * body->line_list.size();

    add_edge(body, back_wheel_part_1, &next_available_vbo_offset);
    add_edge(body, engine, &next_available_vbo_offset);
    add_edge(body, footrest, &next_available_vbo_offset);
    add_edge(body, handlebar, &next_available_vbo_offset);

    add_edge(back_wheel_part_1, back_wheel_part_2, &next_available_vbo_offset);
    add_edge(back_wheel_part_2, back_wheel_part_3, &next_available_vbo_offset);
    add_edge(back_wheel_part_3, back_wheel_spokes, &next_available_vbo_offset);

    add_edge(handlebar, front_scope_1, &next_available_vbo_offset);
    add_edge(handlebar, front_scope_2, &next_available_vbo_offset);

    add_edge(front_scope_1, front_wheel_part_1, &next_available_vbo_offset);

    add_edge(front_wheel_part_1, front_wheel_part_2, &next_available_vbo_offset);
    add_edge(front_wheel_part_2, front_wheel_part_3, &next_available_vbo_offset);
    add_edge(front_wheel_part_3, front_wheel_spokes, &next_available_vbo_offset);
    
    return body;
}

