#include "hnode.hpp"

// Function for making internal connection between front and back faces of obstacles
std::vector<Triangle> facesConnectingTriangles(Point front[], Point back[], int sizeFacePonints, Point color){
    std::vector<Triangle> output;
    for(int i=0, j=0; i<sizeFacePonints; i=i+2, j=j+2)
    {
        output.push_back(Triangle(front[i], back[j], front[i+1], color));
        output.push_back(Triangle(front[i+1], back[j+1], back[j], color));
    }
    return output;
}
// TRACK LOWER
unsigned int sp_track_lower_n = 4, sp_track_lower_k = 64;
std::vector<float> sp_track_lower_rs = {15000, 15000, 14250, 14250}, sp_track_lower_hs = {-710, -680, -650, -620};
std::vector<Point> sp_track_lower_colors = {
    Point(0.2980392156862745, 0.6, 0.5568627450980392), 
    Point(0, 1, 0), 
    Point(0.0, 0.8, 0.8), 
    Point(1, 1, 0), 
    Point(0.2980392156862745, 0.6, 0.5568627450980392),
    Point(1, 0, 0), 
    Point(0, 1, 0), 
    Point(0, 0, 1), 
    Point(1, 1, 0), 
    Point(0, 1, 1),
    Point(1, 0, 0), 
    Point(0, 1, 0), 
    Point(0, 0, 1), 
    Point(1, 1, 0), 
    Point(0, 1, 1),
};
float track_lower_scale[16] = {
    1, 0, 0, 0,
    0, 0.5, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};
glm::mat4 scale_track_lower = glm::make_mat4(track_lower_scale);
glm::mat4 sp_track_lower_init_transform = scale_track_lower * glm::rotate(glm::mat4(1), (glm::mediump_float)(-M_PI/2), glm::vec3(1, 0, 0));
StackedPolyPrism p_track_lower(sp_track_lower_n, sp_track_lower_k, sp_track_lower_rs, sp_track_lower_hs, true, true, sp_track_lower_colors, sp_track_lower_init_transform);

// Start Platform
float start_platform_scale[16] = {
    0.8, 0, 0, 0,
    0, 0.7, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};
glm::mat4 scale_platform = glm::make_mat4(start_platform_scale);
glm::mat4 platform_init_transform = scale_platform * glm::translate(glm::mat4(1), glm::vec3(17000, 4250, -2000)) * glm::rotate(glm::mat4(1), (glm::mediump_float)(M_PI), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), (glm::mediump_float)(-M_PI/6), glm::vec3(0, 1, 0));
float platform_width = 300.0;
float platform_scale = 8.0;
Point startPlatform_front[10] = {
        Point(192.0, -96.0, 0) * platform_scale * platform_init_transform,
        Point(384.0, -96.0, 0) * platform_scale * platform_init_transform,
        Point(192.0, -96.0, 0) * platform_scale * platform_init_transform,
        Point(192.0, -576.0, 0) * platform_scale * platform_init_transform,
        Point(384.0, -96.0, 0) * platform_scale * platform_init_transform,
        Point(1212.0, -576.0, 0) * platform_scale * platform_init_transform,
        Point(192.0, -576.0, 0) * platform_scale * platform_init_transform,
        Point(1212.0, -576.0, 0) * platform_scale * platform_init_transform,
        Point(384.0, -96.0, 0) * platform_scale * platform_init_transform,
        Point(384.0, -576.0, 0) * platform_scale * platform_init_transform
};
Point startPlatform_back[10] = {
        Point(192.0, -96.0, platform_width) * platform_scale * platform_init_transform,
        Point(384.0, -96.0, platform_width) * platform_scale * platform_init_transform,
        Point(192.0, -96.0, platform_width) * platform_scale * platform_init_transform,
        Point(192.0, -576.0, platform_width) * platform_scale * platform_init_transform,
        Point(384.0, -96.0, platform_width) * platform_scale * platform_init_transform,
        Point(1212.0, -576.0, platform_width) * platform_scale * platform_init_transform,
        Point(192.0, -576.0, platform_width) * platform_scale * platform_init_transform,
        Point(1212.0, -576.0, platform_width) * platform_scale * platform_init_transform,
        Point(384.0, -96.0, platform_width) * platform_scale * platform_init_transform,
        Point(384.0, -576.0, platform_width) * platform_scale * platform_init_transform
};
std::vector<Triangle> startPlatform_triangle_list = {
        Triangle(
            startPlatform_front[2],
            startPlatform_front[6],
            startPlatform_front[9],
            Point(0.5725490196078431, 0.058823529411764705, 0.058823529411764705)
        ),
        Triangle(
            startPlatform_front[9],
            startPlatform_front[5],
            startPlatform_front[8],
            Point(0.5725490196078431, 0.058823529411764705, 0.058823529411764705)
        ),
        Triangle(
            startPlatform_front[9],
            startPlatform_front[8],
            startPlatform_front[2],
            Point(0.5725490196078431, 0.058823529411764705, 0.058823529411764705)
        ),
        Triangle(
            startPlatform_back[2],
            startPlatform_back[6],
            startPlatform_back[9],
            Point(0.5725490196078431, 0.058823529411764705, 0.058823529411764705)
        ),
        Triangle(
            startPlatform_back[9],
            startPlatform_back[5],
            startPlatform_back[8],
            Point(0.5725490196078431, 0.058823529411764705, 0.058823529411764705)
        ),
        Triangle(
            startPlatform_back[9],
            startPlatform_back[8],
            startPlatform_back[2],
            Point(0.5725490196078431, 0.058823529411764705, 0.058823529411764705)
        )
};
std::vector<Triangle> startPlatformConnectingTriangles = facesConnectingTriangles(startPlatform_front, startPlatform_back, 10, Point(0.0, 1.0, 0.5019607843137255));

// obstacle1
float obstacle1_scale_mat[16] = {
    1, 0, 0, 0,
    0, 3, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};
glm::mat4 scale_obstacle1 = glm::make_mat4(obstacle1_scale_mat);
glm::mat4 obstacle1_init_transform = scale_obstacle1 * glm::translate(glm::mat4(1), glm::vec3(4500, 2775, -10500)) * glm::rotate(glm::mat4(1), (glm::mediump_float)(M_PI), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1), (glm::mediump_float)(M_PI/2 - (1.5)*M_PI/3), glm::vec3(0, 1, 0));
float obstacle1_width = 100.0;
float obstacle1_scale = 5.00;
Point obstacle1_front[28] = {
        Point(96.0, -576.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(1344.0, -576.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(96.0, -576.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(224.69546508789062,-468.0116882324219,0) * obstacle1_scale * obstacle1_init_transform,
        Point(224.69546508789062,-468.0116882324219, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(312.0,-408.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(312.0,-408.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(384.0,-360.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(384.0,-360.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(499.9111022949219,-328.94171142578125, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(499.9111022949219,-328.94171142578125, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(624.0,-300.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(624.0,-300.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(708.0,-288.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(708.0,-288.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(780.0,-288.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(780.0,-288.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(828.0,-300.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(828.0,-300.0, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(929.48681640625,-336.93817138671875, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(929.48681640625,-336.93817138671875, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(1001.48681640625,-336.93817138671875, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(1001.48681640625,-336.93817138671875, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(1037.349853515625,-340.0757751464844, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(1037.349853515625,-340.0757751464844, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(1127.560302734375,-372.90972900390625, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(1127.560302734375,-372.90972900390625, 0) * obstacle1_scale * obstacle1_init_transform,
        Point(1344.0,-576.0, 0) * obstacle1_scale * obstacle1_init_transform,
    };

Point obstacle1_back[28] = {
        Point(96.0, -576.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(1344.0, -576.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(96.0, -576.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(224.69546508789062,-468.0116882324219,obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(224.69546508789062,-468.0116882324219, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(312.0,-408.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(312.0,-408.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(384.0,-360.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(384.0,-360.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(499.9111022949219,-328.94171142578125, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(499.9111022949219,-328.94171142578125, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(624.0,-300.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(624.0,-300.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(708.0,-288.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(708.0,-288.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(780.0,-288.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(780.0,-288.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(828.0,-300.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(828.0,-300.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(929.48681640625,-336.93817138671875, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(929.48681640625,-336.93817138671875, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(1001.48681640625,-336.93817138671875, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(1001.48681640625,-336.93817138671875, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(1037.349853515625,-340.0757751464844, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(1037.349853515625,-340.0757751464844, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(1127.560302734375,-372.90972900390625, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(1127.560302734375,-372.90972900390625, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
        Point(1344.0,-576.0, obstacle1_width) * obstacle1_scale * obstacle1_init_transform,
    };

std::vector<Triangle> obstacle1_triangle_list = {
        Triangle(
            obstacle1_front[4],
            obstacle1_front[10],
            obstacle1_front[6],
            Point(0,0,1)
        ),
        Triangle(
            obstacle1_front[4],
            obstacle1_front[20],
            obstacle1_front[10],
            Point(0,0,1)
        ),
        Triangle(
            obstacle1_front[8],
            obstacle1_front[6],
            obstacle1_front[10],
            Point(0,0,1)
        ),
        Triangle(
            obstacle1_front[20],
            obstacle1_front[12],
            obstacle1_front[10],
            Point(0,0,1)
        ),
        Triangle(
            obstacle1_front[18],
            obstacle1_front[14],
            obstacle1_front[12],
            Point(0,0,1)
        ),
        Triangle(
            obstacle1_front[27],
            obstacle1_front[4],
            obstacle1_front[2],
            Point(0,0,1)
        ),
        Triangle(
            obstacle1_front[26],
            obstacle1_front[20],
            obstacle1_front[27],
            Point(0,0,1)
        ),
        Triangle(
            obstacle1_front[22],
            obstacle1_front[26],
            obstacle1_front[24],
            Point(0,0,1)
        ),
        Triangle(
            obstacle1_front[16],
            obstacle1_front[14],
            obstacle1_front[18],
            Point(0,0,1)
        ),
        Triangle(
            obstacle1_front[22],
            obstacle1_front[20],
            obstacle1_front[26],
            Point(0,0,1)
        ),
        Triangle(
            obstacle1_front[20],
            obstacle1_front[18],
            obstacle1_front[12],
            Point(0,0,1)
        ),
        Triangle(
            obstacle1_front[20],
            obstacle1_front[4],
            obstacle1_front[27],
            Point(0,0,1)
        ),

        Triangle(
            obstacle1_back[4],
            obstacle1_back[10],
            obstacle1_back[6],
            Point(0, 0, 1)
        ),
        Triangle(
            obstacle1_back[4],
            obstacle1_back[20],
            obstacle1_back[10],
            Point(0, 0, 1)
        ),
        Triangle(
            obstacle1_back[8],
            obstacle1_back[6],
            obstacle1_back[10],
            Point(0, 0, 1)
        ),
        Triangle(
            obstacle1_back[20],
            obstacle1_back[12],
            obstacle1_back[10],
            Point(0, 0, 1)
        ),
        Triangle(
            obstacle1_back[18],
            obstacle1_back[14],
            obstacle1_back[12],
            Point(0, 0, 1)
        ),
        Triangle(
            obstacle1_back[27],
            obstacle1_back[4],
            obstacle1_back[2],
            Point(0, 0, 1)
        ),
        Triangle(
            obstacle1_back[26],
            obstacle1_back[20],
            obstacle1_back[27],
            Point(0, 0, 1)
        ),
        Triangle(
            obstacle1_back[22],
            obstacle1_back[26],
            obstacle1_back[24],
            Point(0, 0, 1)
        ),
        Triangle(
            obstacle1_back[16],
            obstacle1_back[14],
            obstacle1_back[18],
            Point(0, 0, 1)
        ),
        Triangle(
            obstacle1_back[22],
            obstacle1_back[20],
            obstacle1_back[26],
            Point(0, 0, 1)
        ),
        Triangle(
            obstacle1_back[20],
            obstacle1_back[18],
            obstacle1_back[12],
            Point(0, 0, 1)
        ),
        Triangle(
            obstacle1_back[20],
            obstacle1_back[4],
            obstacle1_back[27],
            Point(0, 0, 1)
        )        
};
std::vector<Triangle> obstacle1ConnectingTriangles = facesConnectingTriangles(obstacle1_front, obstacle1_back, 28,Point(0.0, 1.0, 0.5019607843137255));

// obstacle2_1
float obstacle2_1_scale_mat[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};
glm::mat4 scale_obstacle2_1 = glm::make_mat4(obstacle2_1_scale_mat);
glm::mat4 obstacle2_1_init_transform = scale_obstacle2_1 * glm::translate(glm::mat4(1), glm::vec3(-5500, 3150, -10500)) * glm::rotate(glm::mat4(1), (glm::mediump_float)(-M_PI/2-M_PI/5), glm::vec3(0, 1, 0));
float obstacle2_1_width = 300.0;
float obstacle2_1_scale = 6.00;
Point obstacle2_1_front[20] = {
        Point(864.0, -156.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(864.0, -576.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(192.0, -576.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(864.0, -576.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(192.0, -576.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(264.0, -552.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(264.0, -552.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(312.0, -528.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(312.0, -528.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(396.0, -492.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(396.0, -492.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(504.7569274902344, -441.2857971191406, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(504.7569274902344, -441.2857971191406, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(587.8953857421875, -393.2857971191406, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(587.8953857421875, -393.2857971191406, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(684.0, -336.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(684.0, -336.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(768.0, -276.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(768.0, -276.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(864.0, -156.0, 0) * obstacle2_1_scale * obstacle2_1_init_transform
};
Point obstacle2_1_back[20] = {
        Point(864.0, -156.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(864.0, -576.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(192.0, -576.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(864.0, -576.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(192.0, -576.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(264.0, -552.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(264.0, -552.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(312.0, -528.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(312.0, -528.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(396.0, -492.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(396.0, -492.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(504.7569274902344, -441.2857971191406, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(504.7569274902344, -441.2857971191406, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(587.8953857421875, -393.2857971191406, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(587.8953857421875, -393.2857971191406, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(684.0, -336.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(684.0, -336.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(768.0, -276.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(768.0, -276.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform,
        Point(864.0, -156.0, obstacle2_1_width) * obstacle2_1_scale * obstacle2_1_init_transform
};
std::vector<Triangle> obstacle2_1_triangle_list = {
        Triangle(
            obstacle2_1_front[6],
            obstacle2_1_front[4],
            obstacle2_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle2_1_front[1],
            obstacle2_1_front[10],
            obstacle2_1_front[7],
            Point(1,1,1)
        ),
        Triangle(
            obstacle2_1_front[11],
            obstacle2_1_front[10],
            obstacle2_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle2_1_front[7],
            obstacle2_1_front[6],
            obstacle2_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle2_1_front[14],
            obstacle2_1_front[1],
            obstacle2_1_front[15],
            Point(1,1,1)
        ),
        Triangle(
            obstacle2_1_front[1],
            obstacle2_1_front[14],
            obstacle2_1_front[11],
            Point(1,1,1)
        ),
        Triangle(
            obstacle2_1_front[18],
            obstacle2_1_front[15],
            obstacle2_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle2_1_front[0],
            obstacle2_1_front[18],
            obstacle2_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle2_1_back[6],
            obstacle2_1_back[4],
            obstacle2_1_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle2_1_back[1],
            obstacle2_1_back[10],
            obstacle2_1_back[7],
            Point(0,0,1)
        ),
        Triangle(
            obstacle2_1_back[11],
            obstacle2_1_back[10],
            obstacle2_1_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle2_1_back[7],
            obstacle2_1_back[6],
            obstacle2_1_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle2_1_back[14],
            obstacle2_1_back[1],
            obstacle2_1_back[15],
            Point(0,0,1)
        ),
        Triangle(
            obstacle2_1_back[1],
            obstacle2_1_back[14],
            obstacle2_1_back[11],
            Point(0,0,1)
        ),
        Triangle(
            obstacle2_1_back[18],
            obstacle2_1_back[15],
            obstacle2_1_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle2_1_back[0],
            obstacle2_1_back[18],
            obstacle2_1_back[1],
            Point(0,0,1)
        ),
};
std::vector<Triangle> obstacle2_1ConnectingTriangles = facesConnectingTriangles(obstacle2_1_front, obstacle2_1_back, 20, Point(0.0, 1.0, 0.5019607843137255));

// obstacle2_2
float obstacle2_2_scale_mat[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};
glm::mat4 scale_obstacle2_2 = glm::make_mat4(obstacle2_2_scale_mat);
glm::mat4 obstacle2_2_init_transform = scale_obstacle2_2 * glm::translate(glm::mat4(1), glm::vec3(-8500, 3150, -6500)) * glm::rotate(glm::mat4(1), (glm::mediump_float)(-M_PI/2-M_PI/6), glm::vec3(0, 1, 0));
float obstacle2_2_width = 300.0;
float obstacle2_2_scale = 6.00;
Point obstacle2_2_front[6] = {
        Point(672.0,-288.0, 0) * obstacle2_2_scale * obstacle2_2_init_transform,
        Point(672.0,-576.0, 0) * obstacle2_2_scale * obstacle2_2_init_transform,
        Point(672.0,-288.0, 0) * obstacle2_2_scale * obstacle2_2_init_transform,
        Point(1152.0,-576.0, 0) * obstacle2_2_scale * obstacle2_2_init_transform,
        Point(672.0,-576.0, 0) * obstacle2_2_scale * obstacle2_2_init_transform,
        Point(1152.0,-576.0, 0) * obstacle2_2_scale * obstacle2_2_init_transform,
};
Point obstacle2_2_back[6] = {
        Point(672.0,-288.0, obstacle2_2_width) * obstacle2_2_scale * obstacle2_2_init_transform,
        Point(672.0,-576.0, obstacle2_2_width) * obstacle2_2_scale * obstacle2_2_init_transform,
        Point(672.0,-288.0, obstacle2_2_width) * obstacle2_2_scale * obstacle2_2_init_transform,
        Point(1152.0,-576.0, obstacle2_2_width) * obstacle2_2_scale * obstacle2_2_init_transform,
        Point(672.0,-576.0, obstacle2_2_width) * obstacle2_2_scale * obstacle2_2_init_transform,
        Point(1152.0,-576.0, obstacle2_2_width) * obstacle2_2_scale * obstacle2_2_init_transform,
};
std::vector<Triangle> obstacle2_2_triangle_list = {
        Triangle(
            obstacle2_2_front[4],
            obstacle2_2_front[5],
            obstacle2_2_front[0],
            Point(1,1,1)
        ),
        Triangle(
            obstacle2_2_back[4],
            obstacle2_2_back[5],
            obstacle2_2_back[0],
            Point(0,0,1)
        ),
};
std::vector<Triangle> obstacle2_2ConnectingTriangles = facesConnectingTriangles(obstacle2_2_front, obstacle2_2_back, 6, Point(0.0, 1.0, 0.5019607843137255));

// obstacle3_1
float obstacle3_1_scale_mat[16] = {
    1, 0, 0, 0,
    0, 1.5, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};
glm::mat4 scale_obstacle3_1 = glm::make_mat4(obstacle3_1_scale_mat);
glm::mat4 obstacle3_1_init_transform = scale_obstacle3_1 * glm::translate(glm::mat4(1), glm::vec3(-11500, 3250, 300)) * glm::rotate(glm::mat4(1), (glm::mediump_float)(M_PI + M_PI/2), glm::vec3(0, 1, 0));
float obstacle3_1_width = 300.0;
float obstacle3_1_scale = 6.00;
Point obstacle3_1_front[12] = {
        Point(192.0, -576.0, 0) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(480.0, -384.0, 0) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(480.0, -384.0, 0) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(480.0, -576.0, 0) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(480.0, -384.0, 0) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(672.0, -384.0, 0) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(672.0, -576.0, 0) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(672.0, -384.0, 0) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(480.0, -576.0, 0) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(672.0, -576.0, 0) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(192.0, -576.0, 0) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(480.0, -576.0, 0) * obstacle3_1_scale * obstacle3_1_init_transform
};
Point obstacle3_1_back[12] = {
        Point(192.0, -576.0, obstacle3_1_width) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(480.0, -384.0, obstacle3_1_width) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(480.0, -384.0, obstacle3_1_width) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(480.0, -576.0, obstacle3_1_width) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(480.0, -384.0, obstacle3_1_width) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(672.0, -384.0, obstacle3_1_width) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(672.0, -576.0, obstacle3_1_width) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(672.0, -384.0, obstacle3_1_width) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(480.0, -576.0, obstacle3_1_width) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(672.0, -576.0, obstacle3_1_width) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(192.0, -576.0, obstacle3_1_width) * obstacle3_1_scale * obstacle3_1_init_transform,
        Point(480.0, -576.0, obstacle3_1_width) * obstacle3_1_scale * obstacle3_1_init_transform
};
std::vector<Triangle> obstacle3_1_triangle_list = {
        Triangle(
            obstacle3_1_front[10],
            obstacle3_1_front[8],
            obstacle3_1_front[4],
            Point(1,1,1)
        ),
        Triangle(
            obstacle3_1_front[4],
            obstacle3_1_front[6],
            obstacle3_1_front[7],
            Point(1,1,1)
        ),
        Triangle(
            obstacle3_1_front[6],
            obstacle3_1_front[4],
            obstacle3_1_front[8],
            Point(1,1,1)
        ),
        Triangle(
            obstacle3_1_back[10],
            obstacle3_1_back[8],
            obstacle3_1_back[4],
            Point(0,0,1)
        ),
        Triangle(
            obstacle3_1_back[4],
            obstacle3_1_back[6],
            obstacle3_1_back[7],
            Point(0,0,1)
        ),
        Triangle(
            obstacle3_1_back[6],
            obstacle3_1_back[4],
            obstacle3_1_back[8],
            Point(0,0,1)
        )
};
std::vector<Triangle> obstacle3_1ConnectingTriangles = facesConnectingTriangles(obstacle3_1_front, obstacle3_1_back, 12, Point(0.0, 1.0, 0.5019607843137255));

// obstacle3_2
float obstacle3_2_scale_mat[16] = {
    1, 0, 0, 0,
    0, 1.5, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};
glm::mat4 scale_obstacle3_2 = glm::make_mat4(obstacle3_2_scale_mat);
glm::mat4 obstacle3_2_init_transform = scale_obstacle3_2 * glm::translate(glm::mat4(1), glm::vec3(-13500, 3250, 1500)) *  glm::rotate(glm::mat4(1), (glm::mediump_float)(-M_PI/4), glm::vec3(0, 1, 0));
float obstacle3_2_width = 300.0;
float obstacle3_2_scale = 6.0;
Point obstacle3_2_front[12] = {
        Point(384.0, -384.0, 0) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(864.0, -192.0, 0) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(864.0, -192.0, 0) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(864.0, -576.0, 0) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(864.0, -192.0, 0) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(1152.0, -192.0, 0) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(1152.0, -192.0, 0) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(1152.0, -576.0, 0) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(1152.0 ,-576.0, 0) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(864.0, -576.0, 0) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(384.0, -384.0, 0) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(864.0,-384.0, 0) * obstacle3_2_scale * obstacle3_2_init_transform
};
Point obstacle3_2_back[12] = {
        Point(384.0, -384.0, obstacle3_2_width) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(864.0, -192.0, obstacle3_2_width) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(864.0, -192.0, obstacle3_2_width) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(864.0, -576.0, obstacle3_2_width) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(864.0, -192.0, obstacle3_2_width) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(1152.0, -192.0, obstacle3_2_width) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(1152.0, -192.0, obstacle3_2_width) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(1152.0, -576.0, obstacle3_2_width) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(1152.0 ,-576.0, obstacle3_2_width) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(864.0, -576.0, obstacle3_2_width) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(384.0, -384.0, obstacle3_2_width) * obstacle3_2_scale * obstacle3_2_init_transform,
        Point(864.0,-384.0, obstacle3_2_width) * obstacle3_2_scale * obstacle3_2_init_transform
};
std::vector<Triangle> obstacle3_2_triangle_list = {
        Triangle(
            obstacle3_2_front[11],
            obstacle3_2_front[9],
            obstacle3_2_front[7],
            Point(1,1,1)
        ),
        Triangle(
            obstacle3_2_front[10],
            obstacle3_2_front[11],
            obstacle3_2_front[4],
            Point(1,1,1)
        ),
        Triangle(
            obstacle3_2_front[11],
            obstacle3_2_front[5],
            obstacle3_2_front[4],
            Point(1,1,1)
        ),
        Triangle(
            obstacle3_2_front[5],
            obstacle3_2_front[11],
            obstacle3_2_front[7],
            Point(1,1,1)
        ),
        Triangle(
            obstacle3_2_back[11],
            obstacle3_2_back[9],
            obstacle3_2_back[7],
            Point(0,0,1)
        ),
        Triangle(
            obstacle3_2_back[10],
            obstacle3_2_back[11],
            obstacle3_2_back[4],
            Point(0,0,1)
        ),
        Triangle(
            obstacle3_2_back[11],
            obstacle3_2_back[5],
            obstacle3_2_back[4],
            Point(0,0,1)
        ),
        Triangle(
            obstacle3_2_back[5],
            obstacle3_2_back[11],
            obstacle3_2_back[7],
            Point(0,0,1)
        )
};
std::vector<Triangle> obstacle3_2ConnectingTriangles = facesConnectingTriangles(obstacle3_2_front, obstacle3_2_back, 12, Point(0.0, 1.0, 0.5019607843137255));

// obstacle4_1
float obstacle4_1_scale_mat[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};
glm::mat4 scale_obstacle4_1 = glm::make_mat4(obstacle4_1_scale_mat);
glm::mat4 obstacle4_1_init_transform = scale_obstacle4_1 * glm::translate(glm::mat4(1), glm::vec3(-1500, 3125, 13000)) * glm::rotate(glm::mat4(1), (glm::mediump_float)(M_PI-M_PI/6), glm::vec3(0, 1, 0));
float obstacle4_1_width = 300.0;
float obstacle4_1_scale = 6.00;
Point obstacle4_1_front[34] = {
        Point(480.0, -576.0, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(960.0, -576.0, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(480.0, -576.0, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(509.4894714355469, -555.3512573242188, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(509.4894714355469, -555.3512573242188, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(538.9789428710938, -534.7025146484375, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(538.9789428710938, -534.7025146484375, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(570.1558837890625, -516.7025146484375, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(570.1558837890625, -516.7025146484375, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(589.8154907226562, -502.9366760253906, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(589.8154907226562, -502.9366760253906, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(620.992431640625, -484.9366760253906, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(620.992431640625, -484.9366760253906, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(654.8213500976562, -472.6239318847656, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(654.8213500976562, -472.6239318847656, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(684.0, -468.0, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(684.0, -468.0, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(719.863037109375, -464.8623962402344, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(719.863037109375, -464.8623962402344, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(755.863037109375, -464.8623962402344, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(755.863037109375, -464.8623962402344, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(791.7260131835938, -468.0, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(791.7260131835938, -468.0, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(826.4993286132812, -477.3174743652344, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(826.4993286132812, -477.3174743652344, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(859.1264038085938, -492.53173828125, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(859.1264038085938, -492.53173828125, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(888.6159057617188, -513.1804809570312, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(888.6159057617188, -513.1804809570312, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(918.1053466796875, -533.8292236328125, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(918.1053466796875, -533.8292236328125, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(960.5317993164062, -576.2556762695312, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(480.0, -576.0, 0) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(960.5317993164062, -576.2556762695312, 0)  * obstacle4_1_scale * obstacle4_1_init_transform
};
Point obstacle4_1_back[34] = {
        Point(480.0, -576.0, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(960.0, -576.0, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(480.0, -576.0, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(509.4894714355469, -555.3512573242188, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(509.4894714355469, -555.3512573242188, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(538.9789428710938, -534.7025146484375, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(538.9789428710938, -534.7025146484375, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(570.1558837890625, -516.7025146484375, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(570.1558837890625, -516.7025146484375, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(589.8154907226562, -502.9366760253906, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(589.8154907226562, -502.9366760253906, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(620.992431640625, -484.9366760253906, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(620.992431640625, -484.9366760253906, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(654.8213500976562, -472.6239318847656, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(654.8213500976562, -472.6239318847656, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(684.0, -468.0, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(684.0, -468.0, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(719.863037109375, -464.8623962402344, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(719.863037109375, -464.8623962402344, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(755.863037109375, -464.8623962402344, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(755.863037109375, -464.8623962402344, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(791.7260131835938, -468.0, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(791.7260131835938, -468.0, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(826.4993286132812, -477.3174743652344, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(826.4993286132812, -477.3174743652344, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(859.1264038085938, -492.53173828125, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(859.1264038085938, -492.53173828125, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(888.6159057617188, -513.1804809570312, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(888.6159057617188, -513.1804809570312, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(918.1053466796875, -533.8292236328125, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(918.1053466796875, -533.8292236328125, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(960.5317993164062, -576.2556762695312, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(480.0, -576.0, obstacle4_1_width) * obstacle4_1_scale * obstacle4_1_init_transform,
        Point(960.5317993164062, -576.2556762695312, obstacle4_1_width)  * obstacle4_1_scale * obstacle4_1_init_transform
};
std::vector<Triangle> obstacle4_1_triangle_list = {
        Triangle(
            obstacle4_1_front[16],
            obstacle4_1_front[8],
            obstacle4_1_front[18],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[1],
            obstacle4_1_front[8],
            obstacle4_1_front[5],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[4],
            obstacle4_1_front[1],
            obstacle4_1_front[5],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[12],
            obstacle4_1_front[10],
            obstacle4_1_front[16],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[16],
            obstacle4_1_front[14],
            obstacle4_1_front[12],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[10],
            obstacle4_1_front[8],
            obstacle4_1_front[16],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[4],
            obstacle4_1_front[2],
            obstacle4_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[30],
            obstacle4_1_front[28],
            obstacle4_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[1],
            obstacle4_1_front[31],
            obstacle4_1_front[30],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[28],
            obstacle4_1_front[8],
            obstacle4_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[18],
            obstacle4_1_front[8],
            obstacle4_1_front[28],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[18],
            obstacle4_1_front[28],
            obstacle4_1_front[20],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[22],
            obstacle4_1_front[26],
            obstacle4_1_front[24],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[26],
            obstacle4_1_front[22],
            obstacle4_1_front[20],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[28],
            obstacle4_1_front[26],
            obstacle4_1_front[20],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_front[1],
            obstacle4_1_front[2],
            obstacle4_1_front[31],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_1_back[16],
            obstacle4_1_back[8],
            obstacle4_1_back[18],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[1],
            obstacle4_1_back[8],
            obstacle4_1_back[5],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[4],
            obstacle4_1_back[1],
            obstacle4_1_back[5],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[12],
            obstacle4_1_back[10],
            obstacle4_1_back[16],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[16],
            obstacle4_1_back[14],
            obstacle4_1_back[12],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[10],
            obstacle4_1_back[8],
            obstacle4_1_back[16],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[4],
            obstacle4_1_back[2],
            obstacle4_1_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[30],
            obstacle4_1_back[28],
            obstacle4_1_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[1],
            obstacle4_1_back[31],
            obstacle4_1_back[30],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[28],
            obstacle4_1_back[8],
            obstacle4_1_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[18],
            obstacle4_1_back[8],
            obstacle4_1_back[28],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[18],
            obstacle4_1_back[28],
            obstacle4_1_back[20],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[22],
            obstacle4_1_back[26],
            obstacle4_1_back[24],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[26],
            obstacle4_1_back[22],
            obstacle4_1_back[20],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[28],
            obstacle4_1_back[26],
            obstacle4_1_back[20],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_1_back[1],
            obstacle4_1_back[2],
            obstacle4_1_back[31],
            Point(0,0,1)
        ),
};
std::vector<Triangle> obstacle4_1ConnectingTriangles = facesConnectingTriangles(obstacle4_1_front, obstacle4_1_back, 34, Point(0.0, 1.0, 0.5019607843137255));

// obstacle4_2
float obstacle4_2_scale_mat[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};
glm::mat4 scale_obstacle4_2 = glm::make_mat4(obstacle4_2_scale_mat);
glm::mat4 obstacle4_2_init_transform = scale_obstacle4_2 * glm::translate(glm::mat4(1), glm::vec3(670, 3125, 14275)) * glm::rotate(glm::mat4(1), (glm::mediump_float)(M_PI-M_PI/6), glm::vec3(0, 1, 0));
float obstacle4_2_width = 300.0;
float obstacle4_2_scale = 6.00;
Point obstacle4_2_front[34] = {
        Point(480.0, -576.0, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(960.0, -576.0, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(480.0, -576.0, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(509.4894714355469, -555.3512573242188, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(509.4894714355469, -555.3512573242188, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(538.9789428710938, -534.7025146484375, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(538.9789428710938, -534.7025146484375, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(570.1558837890625, -516.7025146484375, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(570.1558837890625, -516.7025146484375, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(589.8154907226562, -502.9366760253906, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(589.8154907226562, -502.9366760253906, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(620.992431640625, -484.9366760253906, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(620.992431640625, -484.9366760253906, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(654.8213500976562, -472.6239318847656, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(654.8213500976562, -472.6239318847656, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(684.0, -468.0, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(684.0, -468.0, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(719.863037109375, -464.8623962402344, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(719.863037109375, -464.8623962402344, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(755.863037109375, -464.8623962402344, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(755.863037109375, -464.8623962402344, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(791.7260131835938, -468.0, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(791.7260131835938, -468.0, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(826.4993286132812, -477.3174743652344, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(826.4993286132812, -477.3174743652344, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(859.1264038085938, -492.53173828125, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(859.1264038085938, -492.53173828125, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(888.6159057617188, -513.1804809570312, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(888.6159057617188, -513.1804809570312, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(918.1053466796875, -533.8292236328125, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(918.1053466796875, -533.8292236328125, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(960.5317993164062, -576.2556762695312, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(480.0, -576.0, 0) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(960.5317993164062, -576.2556762695312, 0)  * obstacle4_2_scale * obstacle4_2_init_transform
};
Point obstacle4_2_back[34] = {
        Point(480.0, -576.0, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(960.0, -576.0, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(480.0, -576.0, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(509.4894714355469, -555.3512573242188, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(509.4894714355469, -555.3512573242188, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(538.9789428710938, -534.7025146484375, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(538.9789428710938, -534.7025146484375, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(570.1558837890625, -516.7025146484375, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(570.1558837890625, -516.7025146484375, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(589.8154907226562, -502.9366760253906, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(589.8154907226562, -502.9366760253906, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(620.992431640625, -484.9366760253906, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(620.992431640625, -484.9366760253906, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(654.8213500976562, -472.6239318847656, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(654.8213500976562, -472.6239318847656, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(684.0, -468.0, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(684.0, -468.0, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(719.863037109375, -464.8623962402344, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(719.863037109375, -464.8623962402344, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(755.863037109375, -464.8623962402344, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(755.863037109375, -464.8623962402344, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(791.7260131835938, -468.0, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(791.7260131835938, -468.0, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(826.4993286132812, -477.3174743652344, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(826.4993286132812, -477.3174743652344, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(859.1264038085938, -492.53173828125, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(859.1264038085938, -492.53173828125, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(888.6159057617188, -513.1804809570312, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(888.6159057617188, -513.1804809570312, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(918.1053466796875, -533.8292236328125, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(918.1053466796875, -533.8292236328125, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(960.5317993164062, -576.2556762695312, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(480.0, -576.0, obstacle4_2_width) * obstacle4_2_scale * obstacle4_2_init_transform,
        Point(960.5317993164062, -576.2556762695312, obstacle4_2_width)  * obstacle4_2_scale * obstacle4_2_init_transform
};
std::vector<Triangle> obstacle4_2_triangle_list = {
        Triangle(
            obstacle4_2_front[16],
            obstacle4_2_front[8],
            obstacle4_2_front[18],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[1],
            obstacle4_2_front[8],
            obstacle4_2_front[5],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[4],
            obstacle4_2_front[1],
            obstacle4_2_front[5],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[12],
            obstacle4_2_front[10],
            obstacle4_2_front[16],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[16],
            obstacle4_2_front[14],
            obstacle4_2_front[12],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[10],
            obstacle4_2_front[8],
            obstacle4_2_front[16],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[4],
            obstacle4_2_front[2],
            obstacle4_2_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[30],
            obstacle4_2_front[28],
            obstacle4_2_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[1],
            obstacle4_2_front[31],
            obstacle4_2_front[30],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[28],
            obstacle4_2_front[8],
            obstacle4_2_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[18],
            obstacle4_2_front[8],
            obstacle4_2_front[28],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[18],
            obstacle4_2_front[28],
            obstacle4_2_front[20],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[22],
            obstacle4_2_front[26],
            obstacle4_2_front[24],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[26],
            obstacle4_2_front[22],
            obstacle4_2_front[20],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[28],
            obstacle4_2_front[26],
            obstacle4_2_front[20],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_front[1],
            obstacle4_2_front[2],
            obstacle4_2_front[31],
            Point(1,1,1)
        ),
        Triangle(
            obstacle4_2_back[16],
            obstacle4_2_back[8],
            obstacle4_2_back[18],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[1],
            obstacle4_2_back[8],
            obstacle4_2_back[5],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[4],
            obstacle4_2_back[1],
            obstacle4_2_back[5],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[12],
            obstacle4_2_back[10],
            obstacle4_2_back[16],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[16],
            obstacle4_2_back[14],
            obstacle4_2_back[12],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[10],
            obstacle4_2_back[8],
            obstacle4_2_back[16],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[4],
            obstacle4_2_back[2],
            obstacle4_2_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[30],
            obstacle4_2_back[28],
            obstacle4_2_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[1],
            obstacle4_2_back[31],
            obstacle4_2_back[30],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[28],
            obstacle4_2_back[8],
            obstacle4_2_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[18],
            obstacle4_2_back[8],
            obstacle4_2_back[28],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[18],
            obstacle4_2_back[28],
            obstacle4_2_back[20],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[22],
            obstacle4_2_back[26],
            obstacle4_2_back[24],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[26],
            obstacle4_2_back[22],
            obstacle4_2_back[20],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[28],
            obstacle4_2_back[26],
            obstacle4_2_back[20],
            Point(0,0,1)
        ),
        Triangle(
            obstacle4_2_back[1],
            obstacle4_2_back[2],
            obstacle4_2_back[31],
            Point(0,0,1)
        ),
};
std::vector<Triangle> obstacle4_2ConnectingTriangles = facesConnectingTriangles(obstacle4_2_front, obstacle4_2_back, 34, Point(0.0, 1.0, 0.5019607843137255));

// obstacle5_1
float obstacle5_1_scale_mat[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};
glm::mat4 scale_obstacle5_1 = glm::make_mat4(obstacle5_1_scale_mat);
glm::mat4 obstacle5_1_init_transform = scale_obstacle5_1 * glm::translate(glm::mat4(1), glm::vec3(7000, 3125, 6750)) *  glm::rotate(glm::mat4(1), (glm::mediump_float)(M_PI - M_PI/6), glm::vec3(0, 1, 0));
float obstacle5_1_width = 300.0;
float obstacle5_1_scale = 6.00;
Point obstacle5_1_front[20] = {
        Point(864.0, -156.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(864.0, -576.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(192.0, -576.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(864.0, -576.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(192.0, -576.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(264.0, -552.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(264.0, -552.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(312.0, -528.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(312.0, -528.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(396.0, -492.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(396.0, -492.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(504.7569274902344, -441.2857971191406, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(504.7569274902344, -441.2857971191406, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(587.8953857421875, -393.2857971191406, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(587.8953857421875, -393.2857971191406, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(684.0, -336.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(684.0, -336.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(768.0, -276.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(768.0, -276.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(864.0, -156.0, 0) * obstacle5_1_scale * obstacle5_1_init_transform
};
Point obstacle5_1_back[20] = {
        Point(864.0, -156.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(864.0, -576.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(192.0, -576.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(864.0, -576.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(192.0, -576.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(264.0, -552.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(264.0, -552.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(312.0, -528.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(312.0, -528.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(396.0, -492.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(396.0, -492.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(504.7569274902344, -441.2857971191406, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(504.7569274902344, -441.2857971191406, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(587.8953857421875, -393.2857971191406, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(587.8953857421875, -393.2857971191406, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(684.0, -336.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(684.0, -336.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(768.0, -276.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(768.0, -276.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform,
        Point(864.0, -156.0, obstacle5_1_width) * obstacle5_1_scale * obstacle5_1_init_transform
};
std::vector<Triangle> obstacle5_1_triangle_list = {
        Triangle(
            obstacle5_1_front[6],
            obstacle5_1_front[4],
            obstacle5_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle5_1_front[1],
            obstacle5_1_front[10],
            obstacle5_1_front[7],
            Point(1,1,1)
        ),
        Triangle(
            obstacle5_1_front[11],
            obstacle5_1_front[10],
            obstacle5_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle5_1_front[7],
            obstacle5_1_front[6],
            obstacle5_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle5_1_front[14],
            obstacle5_1_front[1],
            obstacle5_1_front[15],
            Point(1,1,1)
        ),
        Triangle(
            obstacle5_1_front[1],
            obstacle5_1_front[14],
            obstacle5_1_front[11],
            Point(1,1,1)
        ),
        Triangle(
            obstacle5_1_front[18],
            obstacle5_1_front[15],
            obstacle5_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle5_1_front[0],
            obstacle5_1_front[18],
            obstacle5_1_front[1],
            Point(1,1,1)
        ),
        Triangle(
            obstacle5_1_back[6],
            obstacle5_1_back[4],
            obstacle5_1_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle5_1_back[1],
            obstacle5_1_back[10],
            obstacle5_1_back[7],
            Point(0,0,1)
        ),
        Triangle(
            obstacle5_1_back[11],
            obstacle5_1_back[10],
            obstacle5_1_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle5_1_back[7],
            obstacle5_1_back[6],
            obstacle5_1_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle5_1_back[14],
            obstacle5_1_back[1],
            obstacle5_1_back[15],
            Point(0,0,1)
        ),
        Triangle(
            obstacle5_1_back[1],
            obstacle5_1_back[14],
            obstacle5_1_back[11],
            Point(0,0,1)
        ),
        Triangle(
            obstacle5_1_back[18],
            obstacle5_1_back[15],
            obstacle5_1_back[1],
            Point(0,0,1)
        ),
        Triangle(
            obstacle5_1_back[0],
            obstacle5_1_back[18],
            obstacle5_1_back[1],
            Point(0,0,1)
        ),
};
std::vector<Triangle> obstacle5_1ConnectingTriangles = facesConnectingTriangles(obstacle5_1_front, obstacle5_1_back, 20, Point(0.0, 1.0, 0.5019607843137255));

// obstacle5_2
float obstacle5_2_scale_mat[16] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};
glm::mat4 scale_obstacle5_2 = glm::make_mat4(obstacle5_2_scale_mat);
glm::mat4 obstacle5_2_init_transform = scale_obstacle5_2 * glm::translate(glm::mat4(1), glm::vec3(1000, 3125, 3250)) *  glm::rotate(glm::mat4(1), (glm::mediump_float)(M_PI - M_PI/6), glm::vec3(0, 1, 0));
float obstacle5_2_width = 300.0;
float obstacle5_2_scale = 6.00;
Point obstacle5_2_front[16] = {
        Point(384.0, -576.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(384.0, -420.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(384.0, -420.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(516.0, -348.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(516.0, -348.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(624.0, -288.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(624.0, -288.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1056.0, -288.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1056.0, -288.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1272.0, -420.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1272.0, -420.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1272.0, -576.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(384.0, -576.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1272.0, -576.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(384.0, -420.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1272.0, -420.0, 0) * obstacle5_2_scale * obstacle5_2_init_transform
};
Point obstacle5_2_back[16] = {
        Point(384.0, -576.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(384.0, -420.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(384.0, -420.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(516.0, -348.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(516.0, -348.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(624.0, -288.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(624.0, -288.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1056.0, -288.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1056.0, -288.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1272.0, -420.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1272.0, -420.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1272.0, -576.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(384.0, -576.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1272.0, -576.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(384.0, -420.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform,
        Point(1272.0, -420.0, obstacle5_2_width) * obstacle5_2_scale * obstacle5_2_init_transform
};
std::vector<Triangle> obstacle5_2_triangle_list = {
        Triangle(
            obstacle5_2_front[14],
            obstacle5_2_front[0],
            obstacle5_2_front[13],
            Point(0.09803921568627451, 0.6039215686274509, 0.2)
        ),
        Triangle(
            obstacle5_2_front[3],
            obstacle5_2_front[14],
            obstacle5_2_front[15],
            Point(0.09803921568627451, 0.6039215686274509, 0.2)
        ),
        Triangle(
            obstacle5_2_front[8],
            obstacle5_2_front[3],
            obstacle5_2_front[15],
            Point(0.09803921568627451, 0.6039215686274509, 0.2)
        ),
        Triangle(
            obstacle5_2_front[ 3],
            obstacle5_2_front[8],
            obstacle5_2_front[ 5],
            Point(0.09803921568627451, 0.6039215686274509, 0.2)
        ),
        Triangle(
            obstacle5_2_front[13],
            obstacle5_2_front[15],
            obstacle5_2_front[14],
            Point(0.09803921568627451, 0.6039215686274509, 0.2)
        ),
        Triangle(
            obstacle5_2_back[14],
            obstacle5_2_back[0],
            obstacle5_2_back[13],
            Point(0.09803921568627451, 0.6039215686274509, 0.2)
        ),
        Triangle(
            obstacle5_2_back[3],
            obstacle5_2_back[14],
            obstacle5_2_back[15],
            Point(0.09803921568627451, 0.6039215686274509, 0.2)
        ),
        Triangle(
            obstacle5_2_back[8],
            obstacle5_2_back[3],
            obstacle5_2_back[15],
            Point(0.09803921568627451, 0.6039215686274509, 0.2)
        ),
        Triangle(
            obstacle5_2_back[ 3],
            obstacle5_2_back[8],
            obstacle5_2_back[ 5],
            Point(0.09803921568627451, 0.6039215686274509, 0.2)
        ),
        Triangle(
            obstacle5_2_back[13],
            obstacle5_2_back[15],
            obstacle5_2_back[14],
            Point(0.09803921568627451, 0.6039215686274509, 0.2)
        ),
};
std::vector<Triangle> obstacle5_2ConnectingTriangles = facesConnectingTriangles(obstacle5_2_front, obstacle5_2_back, 16, Point(0.0, 1.0, 0.5019607843137255));

unsigned int sp_n_tris = p_track_lower.triangle_list.size() + startPlatform_triangle_list.size() + startPlatformConnectingTriangles.size() + obstacle1_triangle_list.size() + obstacle1ConnectingTriangles.size() + obstacle2_1_triangle_list.size() + obstacle2_1ConnectingTriangles.size() + obstacle2_2_triangle_list.size() + obstacle2_2ConnectingTriangles.size() + obstacle3_1_triangle_list.size() + obstacle3_1ConnectingTriangles.size() + obstacle3_2_triangle_list.size() + obstacle3_2ConnectingTriangles.size() + obstacle4_1_triangle_list.size() + obstacle4_1ConnectingTriangles.size() + obstacle4_2_triangle_list.size() + obstacle4_2ConnectingTriangles.size() + obstacle5_1_triangle_list.size() + obstacle5_1ConnectingTriangles.size() + obstacle5_2_triangle_list.size() + obstacle5_2ConnectingTriangles.size();
float sp_tri[102400], sp_col[102400];