#include <gtest/gtest.h>

#include "photon/lights/area_light.hpp"
#include "photon/lights/light_sampler.hpp"
#include "photon/samplers/pcg32.hpp"

using photon::lights::AreaLight;
using photon::lights::DistantLight;
using photon::lights::EnvironmentLight;
using photon::lights::LightSampler;
using photon::math::Vec3;
using photon::samplers::PCG32;

TEST(Lights, AreaSamplePdf) {
    AreaLight light{{0.0f, 0.99f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {4.0f, 4.0f, 4.0f}};
    PCG32 rng;
    rng.seed_state(11, 1);
    const auto sample = light.sample({0.5f, 0.0f, 0.5f}, rng);
    EXPECT_GT(sample.pdf, 0.0f);
    EXPECT_GT(sample.distance, 0.0f);
}

TEST(Lights, MISWeight) {
    LightSampler sampler;
    DistantLight sun{{0.3f, -1.0f, 0.2f}, {1.0f, 1.0f, 1.0f}};
    sampler.add(&sun);
    EXPECT_GT(sampler.mis_weight(0.5f, 0.5f), 0.0f);
}

TEST(Lights, EnvironmentCDF) {
    EnvironmentLight env;
    env.width = 2;
    env.height = 2;
    env.pixels = {Vec3{1, 1, 1}, Vec3{0, 0, 0}, Vec3{0, 0, 0}, Vec3{1, 1, 1}};
    env.build_cdf();
    PCG32 rng;
    rng.seed_state(5, 1);
    const auto sample = env.sample({}, rng);
    EXPECT_GT(sample.direction.length(), 0.0f);
}
