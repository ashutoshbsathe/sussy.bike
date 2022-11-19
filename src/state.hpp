#ifndef __STATE_H__
#define __STATE_H__
#include "entity.hpp"
#include "light.hpp"
struct AnimationState {
    std::vector<AnimationEntity> entity_list;
    std::vector<Light> lights_list;
}
#endif

