#pragma once

#include "photon/math/vec3.hpp"

namespace photon::math {

struct Ray {
    Vec3 origin;
    Vec3 direction;
    float t_min = 1e-4f;
    float t_max = 1e30f;

    Ray() = default;
    Ray(const Vec3& o, const Vec3& d, float tmin = 1e-4f, float tmax = 1e30f)
        : origin(o), direction(d.normalized()), t_min(tmin), t_max(tmax) {}

    Vec3 at(float t) const { return origin + direction * t; }
};

}
