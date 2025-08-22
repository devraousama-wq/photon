#include <gtest/gtest.h>

#include "photon/math/aabb.hpp"
#include "photon/math/mat4.hpp"
#include "photon/math/onb.hpp"
#include "photon/math/ray.hpp"
#include "photon/math/vec3.hpp"

using photon::math::AABB;
using photon::math::Mat4;
using photon::math::ONB;
using photon::math::Ray;
using photon::math::Vec3;

TEST(MathVec3, DotAndCross) {
    const Vec3 a{1.0f, 0.0f, 0.0f};
    const Vec3 b{0.0f, 1.0f, 0.0f};
    EXPECT_FLOAT_EQ(a.dot(b), 0.0f);
    const Vec3 c = a.cross(b);
    EXPECT_FLOAT_EQ(c.z, 1.0f);
}

TEST(MathVec3, Normalize) {
    const Vec3 v{3.0f, 0.0f, 4.0f};
    const Vec3 n = v.normalized();
    EXPECT_NEAR(n.length(), 1.0f, 1e-5f);
}

TEST(MathAABB, RayHit) {
    AABB box{{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};
    Ray ray{{0.5f, 0.5f, -1.0f}, {0.0f, 0.0f, 1.0f}};
    float t_enter = 0.0f;
    float t_exit = 0.0f;
    EXPECT_TRUE(box.hit(ray, t_enter, t_exit));
    EXPECT_GT(t_enter, 0.0f);
}

TEST(MathMat4, LookAt) {
    const Mat4 view = Mat4::look_at({0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
    const auto transformed = view.transform_point({0.0f, 0.0f, 0.0f, 1.0f});
    EXPECT_NEAR(transformed.z, 0.0f, 1e-3f);
}

TEST(MathONB, Orthonormal) {
    ONB basis{{0.0f, 1.0f, 0.0f}};
    EXPECT_NEAR(basis.axis[0].dot(basis.axis[1]), 0.0f, 1e-5f);
    EXPECT_NEAR(basis.axis[1].dot(basis.axis[2]), 0.0f, 1e-5f);
}
