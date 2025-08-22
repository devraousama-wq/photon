#pragma once

#include "photon/math/vec3.hpp"

namespace photon::math {

struct ONB {
    Vec3 axis[3];

    ONB() = default;

    explicit ONB(const Vec3& n) {
        axis[2] = n;
        const Vec3 helper = std::fabs(n.x) > 0.9f ? Vec3{0.0f, 1.0f, 0.0f} : Vec3{1.0f, 0.0f, 0.0f};
        axis[1] = axis[2].cross(helper).normalized();
        axis[0] = axis[1].cross(axis[2]);
    }

    Vec3 local(const Vec3& v) const {
        return {
            axis[0].dot(v),
            axis[1].dot(v),
            axis[2].dot(v),
        };
    }

    Vec3 world(float a, float b, float c) const {
        return a * axis[0] + b * axis[1] + c * axis[2];
    }
};

}
