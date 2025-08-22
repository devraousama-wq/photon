#pragma once

#include "photon/math/aabb.hpp"
#include "photon/math/mat4.hpp"
#include "photon/math/onb.hpp"
#include "photon/math/ray.hpp"
#include "photon/math/vec2.hpp"
#include "photon/math/vec3.hpp"
#include "photon/math/vec4.hpp"

namespace photon::math {

inline float radians(float degrees) {
    return degrees * 0.017453292519943295f;
}

inline float clamp(float value, float lo, float hi) {
    return value < lo ? lo : (value > hi ? hi : value);
}

}
