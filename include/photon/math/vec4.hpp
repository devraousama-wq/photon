#pragma once

#include "photon/math/vec3.hpp"

namespace photon::math {

struct Vec4 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;

    constexpr Vec4() = default;
    constexpr Vec4(float px, float py, float pz, float pw) : x(px), y(py), z(pz), w(pw) {}
    explicit constexpr Vec4(const Vec3& v, float pw = 1.0f) : x(v.x), y(v.y), z(v.z), w(pw) {}

    constexpr Vec4 operator+(const Vec4& other) const {
        return {x + other.x, y + other.y, z + other.z, w + other.w};
    }

    constexpr Vec4 operator*(float s) const { return {x * s, y * s, z * s, w * s}; }
    constexpr Vec3 xyz() const { return {x, y, z}; }
};

inline Vec4 operator*(float s, const Vec4& v) { return v * s; }

}
