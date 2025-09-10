#pragma once

#include "photon/geometry/hit_record.hpp"
#include "photon/math/ray.hpp"
#include "photon/math/vec3.hpp"
#include "photon/samplers/pcg32.hpp"

namespace photon::lights {

struct LightSample {
    photon::math::Vec3 direction;
    photon::math::Vec3 radiance;
    float distance = 0.0f;
    float pdf = 0.0f;
};

class Light {
public:
    virtual ~Light() = default;
    virtual LightSample sample(const photon::math::Vec3& point, photon::samplers::PCG32& rng) const = 0;
    virtual float power() const = 0.0f;
};

}
