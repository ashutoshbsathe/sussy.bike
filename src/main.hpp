#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include "point.hpp"
#include "triangle.hpp"
#include "spprism.hpp"
#include "hnode.hpp"
#include "entity.hpp"

#include "gl_framework.hpp"
#include "shader_util.hpp"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// N_CELLS x N_CELLS x N_CELLS cubes
#define N_CELLS 100
// each cube is N_UNITS x N_UNITS x N_UNITS
#define N_UNITS 5

#define CURSOR_PADDING (1.15)
// #define VIEW_PADDING (25.00)

#define DRAW_MIN (-(N_CELLS*N_UNITS)/2)
#define DRAW_MAX ((N_CELLS*N_UNITS)/2)

bool persp = false;
float xrot = 0, yrot = 0, zrot = 0, rotamount = M_PI/12.0, VIEW_PADDING = 3.00, zoomamount = 3.0;
#endif
