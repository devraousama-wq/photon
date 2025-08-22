#pragma once

#include "photon/math/ray.hpp"
#include "photon/math/vec3.hpp"

namespace photon::math {

struct AABB {
    Vec3 min;
    Vec3 max;

    AABB() = default;
    AABB(const Vec3& pmin, const Vec3& pmax) : min(pmin), max(pmax) {}

    static AABB empty() {
        return AABB(
            {1e30f, 1e30f, 1e30f},
            {-1e30f, -1e30f, -1e30f});
    }

    void expand(const Vec3& p) {
        min.x = std::fmin(min.x, p.x);
        min.y = std::fmin(min.y, p.y);
        min.z = std::fmin(min.z, p.z);
        max.x = std::fmax(max.x, p.x);
        max.y = std::fmax(max.y, p.y);
        max.z = std::fmax(max.z, p.z);
    }

    void expand(const AABB& other) {
        expand(other.min);
        expand(other.max);
    }

    Vec3 center() const { return (min + max) * 0.5f; }
    Vec3 extent() const { return max - min; }
    float surface_area() const {
        const Vec3 e = extent();
        return 2.0f * (e.x * e.y + e.y * e.z + e.z * e.x);
    }

    bool hit(const Ray& ray, float& t_enter, float& t_exit) const {
        float t0 = ray.t_min;
        float t1 = ray.t_max;
        for (int axis = 0; axis < 3; ++axis) {
            const float inv_d = 1.0f / ray.direction[axis];
            float t_near = (min[axis] - ray.origin[axis]) * inv_d;
            float t_far = (max[axis] - ray.origin[axis]) * inv_d;
            if (inv_d < 0.0f) {
                std::swap(t_near, t_far);
            }
            t0 = t_near > t0 ? t_near : t0;
            t1 = t_far < t1 ? t_far : t1;
            if (t1 <= t0) {
                return false;
            }
        }
        t_enter = t0;
        t_exit = t1;
        return true;
    }
};

}
