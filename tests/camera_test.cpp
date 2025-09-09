#include <gtest/gtest.h>

#include "photon/camera/camera.hpp"
#include "photon/samplers/pcg32.hpp"

using photon::camera::Camera;
using photon::samplers::PCG32;
using photon::samplers::Sample2D;

TEST(Camera, PinholeRay) {
    Camera cam;
    cam.aspect = 1.0f;
    PCG32 rng;
    rng.seed_state(1, 1);
    const auto ray = cam.generate_ray(Sample2D{0.5f, 0.5f}, rng);
    EXPECT_LT(ray.direction.z, 0.0f);
}

TEST(Camera, DepthOfFieldShift) {
    Camera cam;
    cam.aperture = 0.2f;
    PCG32 rng;
    rng.seed_state(3, 4);
    const auto a = cam.generate_ray(Sample2D{0.25f, 0.75f}, rng);
    rng.seed_state(3, 4);
    const auto b = cam.generate_ray(Sample2D{0.25f, 0.75f}, rng);
    EXPECT_FLOAT_EQ(a.origin.x, b.origin.x);
}
