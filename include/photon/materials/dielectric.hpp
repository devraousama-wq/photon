#pragma once

#include "photon/materials/material.hpp"
#include "photon/math/math.hpp"

#include <cmath>

namespace photon::materials {

class Dielectric : public Material {
public:
    float ior = 1.5f;

    explicit Dielectric(float index) : ior(index) {}

    bool scatter(const photon::math::Ray& ray_in, const photon::geometry::HitRecord& rec, ScatterRecord& srec) const override {
        srec.attenuation = {1.0f, 1.0f, 1.0f};
        const float refraction_ratio = rec.front_face ? (1.0f / ior) : ior;
        const Vec3 unit_direction = ray_in.direction.normalized();
        const float cos_theta = std::fmin((-unit_direction).dot(rec.normal), 1.0f);
        const float sin_theta = std::sqrt(std::fmax(0.0f, 1.0f - cos_theta * cos_theta));
        const bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
        Vec3 direction;
        if (cannot_refract || photon::math::schlick(cos_theta, refraction_ratio) > 0.1f) {
            direction = photon::math::reflect(unit_direction, rec.normal);
        } else {
            direction = photon::math::refract(unit_direction, rec.normal, refraction_ratio);
        }
        srec.scattered = photon::math::Ray(rec.point, direction);
        srec.pdf = 1.0f;
        return true;
    }
};

class Emissive : public Material {
public:
    Vec3 radiance;

    explicit Emissive(const Vec3& emit) : radiance(emit) {}

    bool scatter(const photon::math::Ray&, const photon::geometry::HitRecord&, ScatterRecord& srec) const override {
        srec.absorbed = true;
        return false;
    }

    Vec3 emitted(const photon::geometry::HitRecord&) const override { return radiance; }
};

}
