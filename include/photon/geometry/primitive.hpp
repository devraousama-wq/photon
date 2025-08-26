#pragma once

#include "photon/geometry/hit_record.hpp"
#include "photon/math/ray.hpp"

namespace photon::geometry {

class Primitive {
public:
    virtual ~Primitive() = default;
    virtual bool intersect(const math::Ray& ray, float t_min, float t_max, HitRecord& rec) const = 0;
    virtual math::AABB bounds() const = 0;
};

}
