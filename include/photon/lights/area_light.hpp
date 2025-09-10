#pragma once

#include "photon/lights/area_light.hpp"

namespace photon::lights {

class AreaLight : public Light {
public:
    photon::math::Vec3 corner;
    photon::math::Vec3 u;
    photon::math::Vec3 v;
    photon::math::Vec3 emission;

    AreaLight(const photon::math::Vec3& c, const photon::math::Vec3& u_edge, const photon::math::Vec3& v_edge, const photon::math::Vec3& emit)
        : corner(c), u(u_edge), v(v_edge), emission(emit) {}

    LightSample sample(const photon::math::Vec3& point, photon::samplers::PCG32& rng) const override {
        const float a = rng.next_float();
        const float b = rng.next_float();
        const photon::math::Vec3 on_light = corner + u * a + v * b;
        const photon::math::Vec3 to_light = on_light - point;
        const float dist = to_light.length();
        const photon::math::Vec3 direction = to_light / dist;
        const photon::math::Vec3 normal = u.cross(v).normalized();
        const float cos_theta = std::fabs(normal.dot(-direction));
        const float area = u.cross(v).length();
        const float pdf = (dist * dist) / (area * cos_theta + 1e-6f);
        LightSample sample;
        sample.direction = direction;
        sample.distance = dist;
        sample.radiance = emission;
        sample.pdf = pdf;
        return sample;
    }

    float power() const override {
        return emission.length() * u.cross(v).length() * 3.1415926535f;
    }
};

class DistantLight : public Light {
public:
    photon::math::Vec3 direction;
    photon::math::Vec3 radiance;

    DistantLight(const photon::math::Vec3& dir, const photon::math::Vec3& rad) : direction(dir.normalized()), radiance(rad) {}

    LightSample sample(const photon::math::Vec3&, photon::samplers::PCG32&) const override {
        LightSample sample;
        sample.direction = -direction;
        sample.distance = 1e30f;
        sample.radiance = radiance;
        sample.pdf = 1.0f;
        return sample;
    }

    float power() const override { return radiance.length(); }
};

}
