#include "hnode.hpp"

std::pair<HierarchyNode *, unsigned int> build_track(std::map<std::string, GLuint> gl_info){
    unsigned int next_available_vbo_offset;
    if(gl_info.find("vbo_offset") == gl_info.end()){
        gl_info["vbo_offset"] = 0;
    }
    for(auto it : gl_info){
        std::cout<<it.first<<" : "<<it.second<<"\n";
    }
    next_available_vbo_offset = gl_info["vbo_offset"];
    HierarchyNode *track_lower = new HierarchyNode(StackedPolyPrism("./track_parts/track_lower.txt"));
    HierarchyNode *start_platform = new HierarchyNode("./track_parts/start_platform.txt");
    HierarchyNode *obstacle1 = new HierarchyNode("./track_parts/obstacle1.txt");
    HierarchyNode *obstacle2_1 = new HierarchyNode("./track_parts/obstacle2_1.txt");
    HierarchyNode *obstacle2_2 = new HierarchyNode("./track_parts/obstacle2_2.txt");
    HierarchyNode *obstacle3_1 = new HierarchyNode("./track_parts/obstacle3_1.txt");
    HierarchyNode *obstacle3_2 = new HierarchyNode("./track_parts/obstacle3_2.txt");
    HierarchyNode *obstacle4_1 = new HierarchyNode("./track_parts/obstacle4_1.txt");
    HierarchyNode *obstacle4_2 = new HierarchyNode("./track_parts/obstacle4_2.txt");
    HierarchyNode *obstacle5_1 = new HierarchyNode("./track_parts/obstacle5_1.txt");
    HierarchyNode *obstacle5_2 = new HierarchyNode("./track_parts/obstacle5_2.txt");

    track_lower->make_rigid();
    glm::mat4 track_lower_transform = glm::translate(glm::mat4(1), glm::vec3(0, 350, 0))*glm::rotate(glm::mat4(1), (glm::mediump_float)(-M_PI/2), glm::vec3(1, 0, 0));
    track_lower->private_transform = track_lower_transform;
    start_platform->make_rigid();
    obstacle1->make_rigid();
    obstacle2_1->make_rigid();
    obstacle2_2->make_rigid();
    obstacle3_1->make_rigid();
    obstacle3_2->make_rigid();
    obstacle4_1->make_rigid();
    obstacle4_2->make_rigid();
    obstacle5_1->make_rigid();
    obstacle5_2->make_rigid();

    track_lower->setGLInfo(gl_info);
    
    next_available_vbo_offset += 3 * track_lower->triangle_list.size() + 2 * track_lower->triangle_list.size();
    add_edge(track_lower, start_platform, &next_available_vbo_offset);
    add_edge(track_lower, obstacle1, &next_available_vbo_offset);
    add_edge(track_lower, obstacle2_1, &next_available_vbo_offset);
    add_edge(track_lower, obstacle2_2, &next_available_vbo_offset);
    add_edge(track_lower, obstacle3_1, &next_available_vbo_offset);
    add_edge(track_lower, obstacle3_2, &next_available_vbo_offset);
    add_edge(track_lower, obstacle4_1, &next_available_vbo_offset);
    add_edge(track_lower, obstacle4_2, &next_available_vbo_offset);
    add_edge(track_lower, obstacle5_1, &next_available_vbo_offset);
    add_edge(track_lower, obstacle5_2, &next_available_vbo_offset);

    return std::pair<HierarchyNode *, unsigned int>(track_lower, next_available_vbo_offset);
}
