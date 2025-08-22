#pragma once

#include <cmath>
#include <ostream>

namespace photon::math {

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    constexpr Vec2() = default;
    constexpr Vec2(float px, float py) : x(px), y(py) {}

    constexpr Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
    constexpr Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
    constexpr Vec2 operator*(float s) const { return {x * s, y * s}; }
    constexpr Vec2 operator/(float s) const { return {x / s, y / s}; }

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr float dot(const Vec2& other) const { return x * other.x + y * other.y; }
    float length() const { return std::sqrt(dot(*this)); }
    Vec2 normalized() const {
        const float len = length();
        return len > 0.0f ? *this / len : Vec2{};
    }
};

inline Vec2 operator*(float s, const Vec2& v) { return v * s; }

inline std::ostream& operator<<(std::ostream& out, const Vec2& v) {
    return out << "Vec2(" << v.x << ", " << v.y << ")";
}

}
