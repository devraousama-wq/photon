#pragma once

#include "photon/math/ray.hpp"
#include "photon/math/vec2.hpp"
#include "photon/math/vec3.hpp"

namespace photon::geometry {

struct HitRecord {
    float t = 0.0f;
    Vec3 point;
    Vec3 normal;
    Vec2 uv;
    bool front_face = true;
    int material_id = 0;

    void set_face_normal(const math::Ray& ray, const Vec3& outward_normal) {
        front_face = ray.direction.dot(outward_normal) < 0.0f;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

using photon::math::Vec2;
using photon::math::Vec3;

}
