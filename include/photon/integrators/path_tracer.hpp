#pragma once

#include "photon/accel/bvh.hpp"
#include "photon/geometry/hit_record.hpp"
#include "photon/lights/light_sampler.hpp"
#include "photon/materials/library.hpp"
#include "photon/math/math.hpp"
#include "photon/math/ray.hpp"
#include "photon/math/vec3.hpp"
#include "photon/samplers/pcg32.hpp"

#include <cmath>

namespace photon::integrators {

inline photon::math::Vec3 clamp_vec(const photon::math::Vec3& v, float lo, float hi) {
    return {
        photon::math::clamp(v.x, lo, hi),
        photon::math::clamp(v.y, lo, hi),
        photon::math::clamp(v.z, lo, hi),
    };
}

class PathTracer {
public:
    const photon::accel::BVH* bvh = nullptr;
    const photon::materials::MaterialLibrary* materials = nullptr;
    const photon::lights::LightSampler* lights = nullptr;
    int max_depth = 8;
    bool firefly_clamp = true;

    photon::math::Vec3 trace(const photon::math::Ray& ray, photon::samplers::PCG32& rng) const {
        photon::math::Vec3 throughput{1.0f, 1.0f, 1.0f};
        photon::math::Vec3 radiance{0.0f, 0.0f, 0.0f};
        photon::math::Ray current = ray;

        for (int depth = 0; depth < max_depth; ++depth) {
            photon::geometry::HitRecord rec;
            if (bvh == nullptr || !bvh->intersect(current, 0.001f, 1e30f, rec)) {
                if (lights != nullptr) {
                    const auto env = lights->sample_light(current.origin, rng);
                    if (env.pdf > 0.0f) {
                        radiance += throughput * env.radiance;
                    }
                }
                break;
            }

            const photon::materials::Material* mat = materials != nullptr ? materials->get(rec.material_id) : nullptr;
            if (mat != nullptr) {
                radiance += throughput * mat->emitted(rec);
            }

            if (mat == nullptr) {
                break;
            }

            photon::materials::ScatterRecord scatter;
            if (!mat->scatter(current, rec, scatter) || scatter.absorbed) {
                break;
            }

            if (lights != nullptr && depth < max_depth - 1) {
                const auto ls = lights->sample_light(rec.point, rng);
                if (ls.pdf > 0.0f) {
                    photon::geometry::HitRecord shadow_rec;
                    photon::math::Ray shadow{rec.point, ls.direction};
                    const bool occluded = bvh->intersect(shadow, 0.001f, ls.distance, shadow_rec);
                    if (!occluded) {
                        const float mis = lights->mis_weight(ls.pdf, scatter.pdf);
                        radiance += throughput * scatter.attenuation * ls.radiance * mis / ls.pdf;
                    }
                }
            }

            throughput *= scatter.attenuation;
            current = scatter.scattered;

            if (firefly_clamp) {
                throughput = clamp_vec(throughput, 0.0f, 10.0f);
            }

            const float continue_prob = std::fmin(throughput.length(), 0.95f);
            if (rng.next_float() > continue_prob) {
                break;
            }
            throughput /= continue_prob;
        }

        return clamp_vec(radiance, 0.0f, 1e4f);
    }
};

}
