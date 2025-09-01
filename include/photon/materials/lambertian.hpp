#pragma once

#include "photon/materials/material.hpp"
#include "photon/math/onb.hpp"

#include <cmath>

namespace photon::materials {

class Lambertian : public Material {
public:
    Vec3 albedo;

    explicit Lambertian(const Vec3& color) : albedo(color) {}

    bool scatter(const photon::math::Ray&, const photon::geometry::HitRecord& rec, ScatterRecord& srec) const override {
        photon::math::ONB basis(rec.normal);
        const float u1 = 0.5f;
        const float u2 = 0.5f;
        const float r = std::sqrt(u1);
        const float phi = 2.0f * 3.1415926535f * u2;
        const Vec3 local{r * std::cos(phi), r * std::sin(phi), std::sqrt(std::fmax(0.0f, 1.0f - u1))};
        const Vec3 direction = basis.world(local.x, local.y, local.z);
        srec.scattered = photon::math::Ray(rec.point, direction);
        srec.attenuation = albedo;
        srec.pdf = std::fmax(0.0f, rec.normal.dot(direction)) / 3.1415926535f;
        return true;
    }
};

}
