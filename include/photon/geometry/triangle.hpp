#pragma once

#include "photon/geometry/hit_record.hpp"
#include "photon/geometry/primitive.hpp"

namespace photon::geometry {

class Triangle : public Primitive {
public:
    Vec3 v0;
    Vec3 v1;
    Vec3 v2;
    Vec3 n0;
    Vec3 n1;
    Vec3 n2;
    Vec2 uv0;
    Vec2 uv1;
    Vec2 uv2;
    int material_id = 0;

    Triangle() = default;
    Triangle(const Vec3& a, const Vec3& b, const Vec3& c, int mat = 0)
        : v0(a), v1(b), v2(c), material_id(mat) {
        const Vec3 edge1 = v1 - v0;
        const Vec3 edge2 = v2 - v0;
        const Vec3 n = edge1.cross(edge2).normalized();
        n0 = n1 = n2 = n;
    }

    bool intersect(const math::Ray& ray, float t_min, float t_max, HitRecord& rec) const override {
        const Vec3 edge1 = v1 - v0;
        const Vec3 edge2 = v2 - v0;
        const Vec3 pvec = ray.direction.cross(edge2);
        const float det = edge1.dot(pvec);
        if (std::fabs(det) < 1e-8f) {
            return false;
        }
        const float inv_det = 1.0f / det;
        const Vec3 tvec = ray.origin - v0;
        const float u = tvec.dot(pvec) * inv_det;
        if (u < 0.0f || u > 1.0f) {
            return false;
        }
        const Vec3 qvec = tvec.cross(edge1);
        const float v = ray.direction.dot(qvec) * inv_det;
        if (v < 0.0f || u + v > 1.0f) {
            return false;
        }
        const float t = edge2.dot(qvec) * inv_det;
        if (t < t_min || t > t_max) {
            return false;
        }
        rec.t = t;
        rec.point = ray.at(t);
        const float w = 1.0f - u - v;
        const Vec3 outward = (n0 * w + n1 * u + n2 * v).normalized();
        rec.set_face_normal(ray, outward);
        rec.uv = uv0 * w + uv1 * u + uv2 * v;
        rec.material_id = material_id;
        return true;
    }

    math::AABB bounds() const override {
        math::AABB box = math::AABB::empty();
        box.expand(v0);
        box.expand(v1);
        box.expand(v2);
        return box;
    }
};

}
