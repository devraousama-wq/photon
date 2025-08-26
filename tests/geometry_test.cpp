#include <gtest/gtest.h>

#include "photon/geometry/mesh.hpp"
#include "photon/geometry/sphere.hpp"
#include "photon/geometry/triangle.hpp"
#include "photon/math/ray.hpp"

using photon::geometry::HitRecord;
using photon::geometry::Mesh;
using photon::geometry::SceneObjects;
using photon::geometry::Sphere;
using photon::geometry::Triangle;
using photon::geometry::Vertex;
using photon::math::Ray;
using photon::math::Vec3;

TEST(GeometrySphere, HitCenter) {
    Sphere sphere{{0.0f, 0.0f, 0.0f}, 1.0f};
    HitRecord rec;
    const Ray ray{{0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 1.0f}};
    EXPECT_TRUE(sphere.intersect(ray, 0.001f, 100.0f, rec));
    EXPECT_NEAR(rec.t, 4.0f, 1e-3f);
}

TEST(GeometryTriangle, HitPlane) {
    Triangle tri{{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};
    HitRecord rec;
    const Ray ray{{0.2f, 0.2f, -1.0f}, {0.0f, 0.0f, 1.0f}};
    EXPECT_TRUE(tri.intersect(ray, 0.001f, 100.0f, rec));
}

TEST(GeometryMesh, SceneUnion) {
    SceneObjects scene;
    scene.add(std::make_unique<Sphere>(Vec3{0.0f, 0.0f, 0.0f}, 1.0f));
    Mesh mesh;
    mesh.add_triangle(
        Vertex{{2.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {}},
        Vertex{{3.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {}},
        Vertex{{2.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {}});
    scene.add(std::make_unique<Mesh>(mesh));
    HitRecord rec;
    const Ray ray{{0.0f, 0.0f, -5.0f}, {0.0f, 0.0f, 1.0f}};
    EXPECT_TRUE(scene.intersect(ray, 0.001f, 100.0f, rec));
}
