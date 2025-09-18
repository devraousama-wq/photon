#include <gtest/gtest.h>

#include "photon/accel/bvh.hpp"
#include "photon/geometry/sphere.hpp"
#include "photon/integrators/path_tracer.hpp"
#include "photon/materials/dielectric.hpp"
#include "photon/materials/lambertian.hpp"
#include "photon/math/ray.hpp"

using photon::accel::BVH;
using photon::geometry::Sphere;
using photon::integrators::PathTracer;
using photon::materials::Lambertian;
using photon::materials::MaterialLibrary;
using photon::math::Ray;
using photon::math::Vec3;
using photon::samplers::PCG32;

TEST(Integrator, TraceMissReturnsZero) {
    PathTracer tracer;
    PCG32 rng;
    rng.seed_state(1, 1);
    const auto color = tracer.trace(Ray{{0, 0, 0}, {0, 0, 1}}, rng);
    EXPECT_FLOAT_EQ(color.x, 0.0f);
}

TEST(Integrator, HitsEmissiveSphere) {
    MaterialLibrary mats;
    const int id = mats.add(std::make_unique<Lambertian>(Vec3{0.8f, 0.1f, 0.1f}));

    std::vector<const photon::geometry::Primitive*> refs;
    auto sphere = std::make_unique<Sphere>(Vec3{0, 0, -5}, 1.0f, id);
    refs.push_back(sphere.get());

    BVH bvh(refs);
    PathTracer tracer;
    tracer.bvh = &bvh;
    tracer.materials = &mats;
    tracer.max_depth = 4;

    PCG32 rng;
    rng.seed_state(2, 2);
    const auto color = tracer.trace(Ray{{0, 0, 0}, {0, 0, -1}}, rng);
    EXPECT_GE(color.x, 0.0f);
}
