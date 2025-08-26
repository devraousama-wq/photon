#pragma once

#include "photon/geometry/hit_record.hpp"
#include "photon/geometry/primitive.hpp"

namespace photon::geometry {

class Sphere : public Primitive {
public:
    Vec3 center;
    float radius = 1.0f;
    int material_id = 0;

    Sphere() = default;
    Sphere(const Vec3& c, float r, int mat = 0) : center(c), radius(r), material_id(mat) {}

    bool intersect(const math::Ray& ray, float t_min, float t_max, HitRecord& rec) const override {
        const Vec3 oc = ray.origin - center;
        const float a = ray.direction.length_squared();
        const float half_b = oc.dot(ray.direction);
        const float c = oc.length_squared() - radius * radius;
        const float discriminant = half_b * half_b - a * c;
        if (discriminant < 0.0f) {
            return false;
        }
        const float sqrt_d = std::sqrt(discriminant);
        float root = (-half_b - sqrt_d) / a;
        if (root < t_min || root > t_max) {
            root = (-half_b + sqrt_d) / a;
            if (root < t_min || root > t_max) {
                return false;
            }
        }
        rec.t = root;
        rec.point = ray.at(root);
        const Vec3 outward = (rec.point - center) / radius;
        rec.set_face_normal(ray, outward);
        rec.material_id = material_id;
        return true;
    }

    math::AABB bounds() const override {
        const Vec3 rvec{radius, radius, radius};
        return math::AABB{center - rvec, center + rvec};
    }
};

}
