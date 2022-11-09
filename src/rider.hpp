#include "hnode.hpp"

HierarchyNode *build_humanoid(std::map<std::string, GLuint> gl_info) {
    unsigned int next_available_vbo_offset;
    // Build leaf to root
    HierarchyNode *head = new HierarchyNode(StackedPolyPrism("./body_parts/head.txt"));
    HierarchyNode *neck = new HierarchyNode(StackedPolyPrism("./body_parts/neck.txt"));
    HierarchyNode *palm = new HierarchyNode(StackedPolyPrism("./body_parts/palm.txt"));
	HierarchyNode *wrist = new HierarchyNode(StackedPolyPrism("./body_parts/wrist.txt"));
	HierarchyNode *larm = new HierarchyNode(StackedPolyPrism("./body_parts/larm.txt"));
	HierarchyNode *elbow = new HierarchyNode(StackedPolyPrism("./body_parts/elbow.txt"));
	HierarchyNode *uarm = new HierarchyNode(StackedPolyPrism("./body_parts/uarm.txt"));
	HierarchyNode *shoulder = new HierarchyNode(StackedPolyPrism("./body_parts/shoulder.txt"));
    HierarchyNode *torso_1 = new HierarchyNode(StackedPolyPrism("./body_parts/torso_1.txt"));
	HierarchyNode *torso_2 = new HierarchyNode(StackedPolyPrism("./body_parts/torso_2.txt"));
	HierarchyNode *torso_3 = new HierarchyNode(StackedPolyPrism("./body_parts/torso_3.txt"));
    HierarchyNode *other_palm = new HierarchyNode(StackedPolyPrism("./body_parts/other_palm.txt"));
	HierarchyNode *other_wrist = new HierarchyNode(StackedPolyPrism("./body_parts/other_wrist.txt"));
	HierarchyNode *other_larm = new HierarchyNode(StackedPolyPrism("./body_parts/other_larm.txt"));
	HierarchyNode *other_elbow = new HierarchyNode(StackedPolyPrism("./body_parts/other_elbow.txt"));
	HierarchyNode *other_uarm = new HierarchyNode(StackedPolyPrism("./body_parts/other_uarm.txt"));
	HierarchyNode *other_shoulder = new HierarchyNode(StackedPolyPrism("./body_parts/other_shoulder.txt"));
    HierarchyNode *feet = new HierarchyNode(StackedPolyPrism("./body_parts/feet.txt"));
	HierarchyNode *ankle = new HierarchyNode(StackedPolyPrism("./body_parts/ankle.txt"));
	HierarchyNode *lthigh = new HierarchyNode(StackedPolyPrism("./body_parts/lthigh.txt"));
	HierarchyNode *knee = new HierarchyNode(StackedPolyPrism("./body_parts/knee.txt"));
	HierarchyNode *uthigh = new HierarchyNode(StackedPolyPrism("./body_parts/uthigh.txt"));
	HierarchyNode *hip = new HierarchyNode(StackedPolyPrism("./body_parts/hip.txt"));
    HierarchyNode *other_feet = new HierarchyNode(StackedPolyPrism("./body_parts/other_feet.txt"));
	HierarchyNode *other_ankle = new HierarchyNode(StackedPolyPrism("./body_parts/other_ankle.txt"));
	HierarchyNode *other_lthigh = new HierarchyNode(StackedPolyPrism("./body_parts/other_lthigh.txt"));
	HierarchyNode *other_knee = new HierarchyNode(StackedPolyPrism("./body_parts/other_knee.txt"));
	HierarchyNode *other_uthigh = new HierarchyNode(StackedPolyPrism("./body_parts/other_uthigh.txt"));
	HierarchyNode *other_hip = new HierarchyNode(StackedPolyPrism("./body_parts/other_hip.txt"));

    // dummy node for testing light
    HierarchyNode *light = new HierarchyNode(StackedPolyPrism("./body_parts/light.txt"));
    light->make_rigid();
    HierarchyNode *floor = new HierarchyNode(StackedPolyPrism("./body_parts/floor.txt"));
    floor->make_rigid();

    head->make_rigid();
    uarm->make_rigid();
    larm->make_rigid();
    palm->make_rigid();
    uthigh->make_rigid();
    lthigh->make_rigid();
    feet->make_rigid();
    other_uarm->make_rigid();
    other_larm->make_rigid();
    other_palm->make_rigid();
    other_uthigh->make_rigid();
    other_lthigh->make_rigid();
    other_feet->make_rigid();

    torso_1->n_dof = 3;
    torso_1->dof_limits = {
        {-M_PI/12., M_PI/12.},
        {-M_PI/24., M_PI/24.},
        {-M_PI/36., M_PI/36.},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    torso_1->dof_deltas = {M_PI/36, M_PI/36, M_PI/36, 0, 0, 0};

    torso_3->n_dof = 3;
    torso_3->dof_limits = {
        {-M_PI/12., M_PI/12.},
        {-M_PI/24., M_PI/24.},
        {-M_PI/36., M_PI/36.},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    torso_3->dof_deltas = {M_PI/36, M_PI/36, M_PI/36, 0, 0, 0};

    neck->n_dof = 3;
    neck->dof_limits = {
        {-M_PI/2, M_PI/2},
        {-M_PI/2, M_PI/2},
        {-M_PI/2, M_PI/2},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    neck->dof_deltas = {M_PI/36, M_PI/36, M_PI/36, 0, 0, 0};
    neck->dof_params = {
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };

    shoulder->n_dof = 3;
    shoulder->dof_limits = {
        {-M_PI, M_PI},
        {-M_PI, M_PI},
        {-M_PI, M_PI},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    shoulder->dof_deltas = {M_PI/36, M_PI/36, M_PI/36, 0, 0, 0};
    shoulder->dof_params = {
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
    elbow->n_dof = 1;
    elbow->dof_limits = {
        {-M_PI/2, 0},
        {0, 0},
        {0, 0},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    elbow->dof_deltas = {M_PI/36, 0, 0, 0, 0, 0};
    elbow->dof_params = {
        {-M_PI/2, glm::vec3(0, 1, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
    wrist->n_dof = 3;
    wrist->dof_limits = {
        {-M_PI/2, M_PI/2},
        {-M_PI/2, M_PI/2},
        {-M_PI/2, M_PI/2},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    wrist->dof_deltas = {M_PI/36, M_PI/36, M_PI/36, 0, 0, 0};
    wrist->dof_params = {
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
    hip->n_dof = 3;
    hip->dof_limits = {
        {-M_PI, M_PI},
        {-M_PI, M_PI},
        {-M_PI, M_PI},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    hip->dof_deltas = {M_PI/36, M_PI/36, M_PI/36, 0, 0, 0};
    hip->dof_params = {
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
    knee->n_dof = 1;
    knee->dof_limits = {
        {-M_PI/2, 0},
        {0, 0},
        {0, 0},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    knee->dof_deltas = {M_PI/36, 0, 0, 0, 0, 0};
    knee->dof_params = {
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
    ankle->n_dof = 3;
    ankle->dof_limits = {
        {-M_PI/2, M_PI/2},
        {-M_PI/2, M_PI/2},
        {-M_PI/2, M_PI/2},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    ankle->dof_deltas = {M_PI/36, M_PI/36, M_PI/36, 0, 0, 0};
    ankle->dof_params = {
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };

    other_shoulder->n_dof = 3;
    other_shoulder->dof_limits = {
        {-M_PI, M_PI},
        {-M_PI, M_PI},
        {-M_PI, M_PI},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    other_shoulder->dof_deltas = {M_PI/36, M_PI/36, M_PI/36, 0, 0, 0};
    other_shoulder->dof_params = {
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
    other_elbow->n_dof = 1;
    other_elbow->dof_limits = {
        {-M_PI/2, 0},
        {0, 0},
        {0, 0},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    other_elbow->dof_deltas = {M_PI/36, 0, 0, 0, 0, 0};
    other_elbow->dof_params = {
        {-M_PI/2, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
    other_wrist->n_dof = 3;
    other_wrist->dof_limits = {
        {-M_PI/2, M_PI/2},
        {-M_PI/2, M_PI/2},
        {-M_PI/2, M_PI/2},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    other_wrist->dof_deltas = {M_PI/36, M_PI/36, M_PI/36, 0, 0, 0};
    other_wrist->dof_params = {
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
    other_hip->n_dof = 3;
    other_hip->dof_limits = {
        {-M_PI, M_PI},
        {-M_PI, M_PI},
        {-M_PI, M_PI},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    other_hip->dof_deltas = {M_PI/36, M_PI/36, M_PI/36, 0, 0, 0};
    other_hip->dof_params = {
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
    other_knee->n_dof = 1;
    other_knee->dof_limits = {
        {-M_PI/2, 0},
        {0, 0},
        {0, 0},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    other_knee->dof_deltas = {M_PI/36, 0, 0, 0, 0, 0};
    other_knee->dof_params = {
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };
    other_ankle->n_dof = 3;
    other_ankle->dof_limits = {
        {-M_PI/2, M_PI/2},
        {-M_PI/2, M_PI/2},
        {-M_PI/2, M_PI/2},
        {-1024000, 1024000},
        {-1024000, 1024000},
        {-1024000, 1024000},
    };
    other_ankle->dof_deltas = {M_PI/36, M_PI/36, M_PI/36, 0, 0, 0};
    other_ankle->dof_params = {
        {0, glm::vec3(1, 0, 0)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 0, 1)},
        {0, glm::vec3(0, 1, 0)},
        {0, glm::vec3(1, 0, 0)},
    };

    next_available_vbo_offset = 3 * torso_2->triangle_list.size();
    
    float torso_flatten[16] = {
        1, 0, 0, 0,
        0, 0.65, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    torso_1->private_transform = glm::make_mat4(torso_flatten);
    torso_2->private_transform = glm::make_mat4(torso_flatten);
    torso_3->private_transform = glm::make_mat4(torso_flatten);
    torso_2->vbo_offset = 0;
    torso_2->setGLInfo(gl_info);
    add_edge(torso_2, torso_1, &next_available_vbo_offset);
    add_edge(torso_2, torso_3, &next_available_vbo_offset);

    add_edge(torso_1, neck, &next_available_vbo_offset);
    add_edge(torso_1, shoulder, &next_available_vbo_offset);
    add_edge(torso_1, other_shoulder, &next_available_vbo_offset);

    add_edge(torso_3, hip, &next_available_vbo_offset);
    add_edge(torso_3, other_hip, &next_available_vbo_offset);

    add_edge(neck, head, &next_available_vbo_offset);

    add_edge(shoulder, uarm, &next_available_vbo_offset);
    add_edge(uarm, elbow, &next_available_vbo_offset);
    add_edge(elbow, larm, &next_available_vbo_offset);
    add_edge(larm, wrist, &next_available_vbo_offset);
    add_edge(wrist, palm, &next_available_vbo_offset);

    add_edge(hip, uthigh, &next_available_vbo_offset);
    add_edge(uthigh, knee, &next_available_vbo_offset);
    add_edge(knee, lthigh, &next_available_vbo_offset);
    add_edge(lthigh, ankle, &next_available_vbo_offset);
    add_edge(ankle, feet, &next_available_vbo_offset);

    add_edge(other_shoulder, other_uarm, &next_available_vbo_offset);
    add_edge(other_uarm, other_elbow, &next_available_vbo_offset);
    add_edge(other_elbow, other_larm, &next_available_vbo_offset);
    add_edge(other_larm, other_wrist, &next_available_vbo_offset);
    add_edge(other_wrist, other_palm, &next_available_vbo_offset);

    add_edge(other_hip, other_uthigh, &next_available_vbo_offset);
    add_edge(other_uthigh, other_knee, &next_available_vbo_offset);
    add_edge(other_knee, other_lthigh, &next_available_vbo_offset);
    add_edge(other_lthigh, other_ankle, &next_available_vbo_offset);
    add_edge(other_ankle, other_feet, &next_available_vbo_offset);

    //add_edge(torso_2, light, &next_available_vbo_offset);
    add_edge(torso_2, floor, &next_available_vbo_offset);
    return torso_2;
}

