#include <gtest/gtest.h>

#include "photon/geometry/hit_record.hpp"
#include "photon/materials/dielectric.hpp"
#include "photon/materials/ggx.hpp"
#include "photon/materials/lambertian.hpp"
#include "photon/materials/library.hpp"
#include "photon/math/ray.hpp"

using photon::geometry::HitRecord;
using photon::materials::Dielectric;
using photon::materials::Emissive;
using photon::materials::GGXMicrofacet;
using photon::materials::Lambertian;
using photon::materials::MaterialLibrary;
using photon::materials::ScatterRecord;
using photon::math::Ray;
using photon::math::Vec3;

TEST(Materials, LambertianScatter) {
    Lambertian mat{{0.8f, 0.2f, 0.1f}};
    HitRecord rec;
    rec.point = {0.0f, 0.0f, 0.0f};
    rec.normal = {0.0f, 1.0f, 0.0f};
    rec.front_face = true;
    ScatterRecord srec;
    EXPECT_TRUE(mat.scatter(Ray{{0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}, rec, srec));
    EXPECT_GT(srec.pdf, 0.0f);
}

TEST(Materials, DielectricReflection) {
    Dielectric glass{1.5f};
    HitRecord rec;
    rec.point = {0.0f, 0.0f, 0.0f};
    rec.normal = {0.0f, 1.0f, 0.0f};
    rec.front_face = true;
    ScatterRecord srec;
    EXPECT_TRUE(glass.scatter(Ray{{0.0f, 1.0f, -1.0f}, Vec3{0.0f, -0.5f, 0.5f}.normalized()}, rec, srec));
}

TEST(Materials, LibraryLookup) {
    MaterialLibrary library;
    library.add(std::make_unique<Lambertian>(Vec3{0.5f, 0.5f, 0.5f}));
    library.add(std::make_unique<GGXMicrofacet>(Vec3{1.0f, 1.0f, 1.0f}, 0.2f, 1.0f));
    library.add(std::make_unique<Emissive>(Vec3{4.0f, 4.0f, 4.0f}));
    EXPECT_NE(library.get(2), nullptr);
}
