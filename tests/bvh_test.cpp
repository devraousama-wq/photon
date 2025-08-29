#include <gtest/gtest.h>

#include "photon/accel/bvh.hpp"
#include "photon/geometry/mesh.hpp"
#include "photon/geometry/sphere.hpp"
#include "photon/math/ray.hpp"

using photon::accel::BVH;
using photon::geometry::HitRecord;
using photon::geometry::SceneObjects;
using photon::geometry::Sphere;
using photon::math::Ray;
using photon::math::Vec3;

TEST(BVH, IntersectsSphere) {
    SceneObjects scene;
    scene.add(std::make_unique<Sphere>(Vec3{0.0f, 0.0f, 0.0f}, 1.0f));
    scene.add(std::make_unique<Sphere>(Vec3{3.0f, 0.0f, 0.0f}, 0.5f));

    std::vector<const photon::geometry::Primitive*> refs;
    for (const auto& obj : scene.objects) {
        refs.push_back(obj.get());
    }

    BVH tree(refs);
    HitRecord rec;
    const Ray ray{{0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 1.0f}};
    EXPECT_TRUE(tree.intersect(ray, 0.001f, 100.0f, rec));
    EXPECT_NEAR(rec.t, 4.0f, 1e-2f);
}
