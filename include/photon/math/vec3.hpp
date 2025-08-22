#pragma once

#include <cmath>
#include <ostream>

namespace photon::math {

struct Vec3 {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    constexpr Vec3() = default;
    constexpr Vec3(float px, float py, float pz) : x(px), y(py), z(pz) {}

    constexpr Vec3 operator+(const Vec3& other) const { return {x + other.x, y + other.y, z + other.z}; }
    constexpr Vec3 operator-(const Vec3& other) const { return {x - other.x, y - other.y, z - other.z}; }
    constexpr Vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
    constexpr Vec3 operator/(float s) const { return {x / s, y / s, z / s}; }
    constexpr Vec3 operator-() const { return {-x, -y, -z}; }

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    constexpr float dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }
    constexpr Vec3 cross(const Vec3& other) const {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x,
        };
    }

    float length_squared() const { return dot(*this); }
    float length() const { return std::sqrt(length_squared()); }
    Vec3 normalized() const {
        const float len = length();
        return len > 0.0f ? *this / len : Vec3{};
    }

    constexpr float operator[](int i) const { return i == 0 ? x : (i == 1 ? y : z); }
    constexpr float& operator[](int i) { return i == 0 ? x : (i == 1 ? y : z); }
};

inline Vec3 operator*(float s, const Vec3& v) { return v * s; }

inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
    return out << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2.0f * v.dot(n) * n;
}

inline Vec3 refract(const Vec3& uv, const Vec3& n, float eta_ratio) {
    const float cos_theta = std::fmin((-uv).dot(n), 1.0f);
    const Vec3 r_out_perp = eta_ratio * (uv + cos_theta * n);
    const float discriminant = 1.0f - eta_ratio * eta_ratio * (1.0f - cos_theta * cos_theta);
    if (discriminant < 0.0f) {
        return {};
    }
    const Vec3 r_out_parallel = -std::sqrt(discriminant) * n;
    return r_out_perp + r_out_parallel;
}

inline float schlick(float cosine, float ref_idx) {
    float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::pow(1.0f - cosine, 5.0f);
}

}
