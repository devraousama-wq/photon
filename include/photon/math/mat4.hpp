#pragma once

#include "photon/math/vec3.hpp"
#include "photon/math/vec4.hpp"

#include <array>
#include <cmath>

namespace photon::math {

struct Mat4 {
    std::array<float, 16> m{};

    static Mat4 identity() {
        Mat4 out;
        out.m = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };
        return out;
    }

    static Mat4 translate(const Vec3& t) {
        Mat4 out = identity();
        out.m[12] = t.x;
        out.m[13] = t.y;
        out.m[14] = t.z;
        return out;
    }

    static Mat4 scale(const Vec3& s) {
        Mat4 out = identity();
        out.m[0] = s.x;
        out.m[5] = s.y;
        out.m[10] = s.z;
        return out;
    }

    static Mat4 perspective(float fov_y, float aspect, float z_near, float z_far) {
        const float tan_half = std::tan(fov_y * 0.5f);
        Mat4 out{};
        out.m[0] = 1.0f / (aspect * tan_half);
        out.m[5] = 1.0f / tan_half;
        out.m[10] = -(z_far + z_near) / (z_far - z_near);
        out.m[11] = -1.0f;
        out.m[14] = -(2.0f * z_far * z_near) / (z_far - z_near);
        return out;
    }

    static Mat4 look_at(const Vec3& eye, const Vec3& target, const Vec3& up) {
        const Vec3 f = (target - eye).normalized();
        const Vec3 r = f.cross(up).normalized();
        const Vec3 u = r.cross(f);
        Mat4 out = identity();
        out.m[0] = r.x;
        out.m[1] = u.x;
        out.m[2] = -f.x;
        out.m[4] = r.y;
        out.m[5] = u.y;
        out.m[6] = -f.y;
        out.m[8] = r.z;
        out.m[9] = u.z;
        out.m[10] = -f.z;
        out.m[12] = -r.dot(eye);
        out.m[13] = -u.dot(eye);
        out.m[14] = f.dot(eye);
        return out;
    }

    Mat4 operator*(const Mat4& other) const {
        Mat4 out{};
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                float sum = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    sum += m[row + k * 4] * other.m[k + col * 4];
                }
                out.m[row + col * 4] = sum;
            }
        }
        return out;
    }

    Vec4 transform_point(const Vec4& v) const {
        const float x = m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w;
        const float y = m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w;
        const float z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
        const float w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
        return {x, y, z, w};
    }

    Vec3 transform_direction(const Vec3& v) const {
        const float x = m[0] * v.x + m[4] * v.y + m[8] * v.z;
        const float y = m[1] * v.x + m[5] * v.y + m[9] * v.z;
        const float z = m[2] * v.x + m[6] * v.y + m[10] * v.z;
        return {x, y, z};
    }
};

}
