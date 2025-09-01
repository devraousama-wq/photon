#pragma once

#include "photon/materials/material.hpp"

#include <cmath>

namespace photon::materials {

inline float ggx_distribution(float n_dot_h, float roughness) {
    const float a = roughness * roughness;
    const float a2 = a * a;
    const float denom = n_dot_h * n_dot_h * (a2 - 1.0f) + 1.0f;
    return a2 / (3.1415926535f * denom * denom + 1e-6f);
}

inline float smith_g1(float n_dot_v, float roughness) {
    const float r = roughness + 1.0f;
    const float k = (r * r) / 8.0f;
    return n_dot_v / (n_dot_v * (1.0f - k) + k + 1e-6f);
}

class GGXMicrofacet : public Material {
public:
    Vec3 albedo;
    float roughness = 0.5f;
    float metallic = 0.0f;

    GGXMicrofacet(const Vec3& color, float rough, float metal)
        : albedo(color), roughness(rough), metallic(metal) {}

    bool scatter(const photon::math::Ray& ray_in, const photon::geometry::HitRecord& rec, ScatterRecord& srec) const override {
        const Vec3 view = (-ray_in.direction).normalized();
        const Vec3 h = (view + photon::math::reflect(-view, rec.normal)).normalized();
        const float n_dot_h = std::fmax(0.0f, rec.normal.dot(h));
        const float n_dot_v = std::fmax(0.0f, rec.normal.dot(view));
        const float d = ggx_distribution(n_dot_h, roughness);
        const float g = smith_g1(n_dot_v, roughness) * smith_g1(n_dot_h, roughness);
        const float spec = (d * g) / (4.0f * n_dot_v * n_dot_h + 1e-6f);
        const Vec3 f0 = albedo * metallic + Vec3{0.04f, 0.04f, 0.04f} * (1.0f - metallic);
        const Vec3 spec_color = f0 + (Vec3{1.0f, 1.0f, 1.0f} - f0) * std::pow(1.0f - view.dot(h), 5.0f);
        const Vec3 direction = photon::math::reflect(ray_in.direction, rec.normal);
        srec.scattered = photon::math::Ray(rec.point, direction);
        srec.attenuation = spec_color * spec;
        srec.pdf = d;
        return true;
    }
};

}
