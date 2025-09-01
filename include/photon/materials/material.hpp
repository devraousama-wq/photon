#pragma once

#include "photon/geometry/hit_record.hpp"
#include "photon/math/ray.hpp"
#include "photon/math/vec3.hpp"

namespace photon::materials {

using photon::math::Vec3;

struct ScatterRecord {
    photon::math::Ray scattered;
    Vec3 attenuation;
    float pdf = 0.0f;
    bool absorbed = false;
};

class Material {
public:
    virtual ~Material() = default;
    virtual bool scatter(const photon::math::Ray& ray_in, const photon::geometry::HitRecord& rec, ScatterRecord& srec) const = 0;
    virtual Vec3 emitted(const photon::geometry::HitRecord& rec) const { return {}; }
};

}
