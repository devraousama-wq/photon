#pragma once

#include "photon/math/math.hpp"
#include "photon/math/ray.hpp"
#include "photon/math/vec3.hpp"
#include "photon/samplers/pcg32.hpp"

#include <cmath>

namespace photon::camera {

class Camera {
public:
    photon::math::Vec3 origin{0.0f, 1.0f, 5.0f};
    photon::math::Vec3 look_at{0.0f, 0.0f, 0.0f};
    photon::math::Vec3 up{0.0f, 1.0f, 0.0f};
    float vertical_fov = 45.0f;
    float aspect = 1.0f;
    float aperture = 0.0f;
    float focus_distance = 5.0f;

    photon::math::Ray generate_ray(const photon::samplers::Sample2D& sample, photon::samplers::PCG32& rng) const {
        const float theta = photon::math::radians(vertical_fov);
        const float h = std::tan(theta * 0.5f);
        const float viewport_height = 2.0f * h * focus_distance;
        const float viewport_width = aspect * viewport_height;

        const photon::math::Vec3 w = (origin - look_at).normalized();
        const photon::math::Vec3 u = up.cross(w).normalized();
        const photon::math::Vec3 v = w.cross(u);

        const photon::math::Vec3 horizontal = u * viewport_width;
        const photon::math::Vec3 vertical = v * viewport_height;
        const photon::math::Vec3 lower_left = origin - horizontal * 0.5f - vertical * 0.5f - focus_distance * w;

        const float s = sample.u * 2.0f - 1.0f;
        const float t = 1.0f - sample.v * 2.0f;
        photon::math::Vec3 target = lower_left + horizontal * ((s + 1.0f) * 0.5f) + vertical * ((t + 1.0f) * 0.5f);

        photon::math::Vec3 lens_origin = origin;
        if (aperture > 0.0f) {
            const float r1 = rng.next_float();
            const float r2 = rng.next_float();
            const float radius = aperture * std::sqrt(r1);
            const float theta_r = 2.0f * 3.1415926535f * r2;
            const photon::math::Vec3 offset = u * (radius * std::cos(theta_r)) + v * (radius * std::sin(theta_r));
            lens_origin += offset;
            target = origin + (target - origin).normalized() * focus_distance;
        }

        const photon::math::Vec3 direction = (target - lens_origin).normalized();
        return photon::math::Ray(lens_origin, direction);
    }
};

}
