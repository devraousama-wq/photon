#pragma once

#include "photon/lights/light.hpp"
#include "photon/math/vec3.hpp"

#include <algorithm>
#include <vector>

namespace photon::lights {

class EnvironmentLight : public Light {
public:
    int width = 0;
    int height = 0;
    std::vector<photon::math::Vec3> pixels;
    std::vector<float> cdf;
    float total_luminance = 0.0f;

    void build_cdf() {
        cdf.resize(pixels.size() + 1);
        cdf[0] = 0.0f;
        total_luminance = 0.0f;
        for (std::size_t i = 0; i < pixels.size(); ++i) {
            const float lum = pixels[i].x * 0.2126f + pixels[i].y * 0.7152f + pixels[i].z * 0.0722f;
            total_luminance += lum;
            cdf[i + 1] = cdf[i] + lum;
        }
        if (total_luminance > 0.0f) {
            for (float& v : cdf) {
                v /= total_luminance;
            }
        }
    }

    LightSample sample(const photon::math::Vec3&, photon::samplers::PCG32& rng) const override {
        const float u = rng.next_float();
        const auto it = std::lower_bound(cdf.begin(), cdf.end(), u);
        const std::size_t index = static_cast<std::size_t>(std::distance(cdf.begin(), it));
        const std::size_t clamped = index == 0 ? 0 : index - 1;
        const int y = static_cast<int>(clamped / width);
        const int x = static_cast<int>(clamped % width);
        const float theta = (static_cast<float>(y) + 0.5f) / static_cast<float>(height) * 3.1415926535f;
        const float phi = (static_cast<float>(x) + 0.5f) / static_cast<float>(width) * 2.0f * 3.1415926535f;
        const float sin_theta = std::sin(theta);
        photon::math::Vec3 direction{
            sin_theta * std::cos(phi),
            std::cos(theta),
            sin_theta * std::sin(phi),
        };
        LightSample sample;
        sample.direction = direction;
        sample.distance = 1e30f;
        sample.radiance = pixels[clamped];
        sample.pdf = (total_luminance / static_cast<float>(pixels.size())) / (2.0f * 3.1415926535f * 3.1415926535f * sin_theta + 1e-6f);
        return sample;
    }

    float power() const override { return total_luminance; }
};

class LightSampler {
public:
    std::vector<const Light*> lights;

    void add(const Light* light) { lights.push_back(light); }

    LightSample sample_light(const photon::math::Vec3& point, photon::samplers::PCG32& rng) const {
        if (lights.empty()) {
            return {};
        }
        float total = 0.0f;
        for (const auto* light : lights) {
            total += light->power();
        }
        float pick = rng.next_float() * total;
        const Light* chosen = lights.back();
        for (const auto* light : lights) {
            pick -= light->power();
            if (pick <= 0.0f) {
                chosen = light;
                break;
            }
        }
        return chosen->sample(point, rng);
    }

    float mis_weight(float light_pdf, float brdf_pdf) const {
        const float sum = light_pdf * light_pdf + brdf_pdf * brdf_pdf;
        return sum > 0.0f ? (light_pdf * light_pdf) / sum : 0.0f;
    }
};

}
