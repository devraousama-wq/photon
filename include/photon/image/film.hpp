#pragma once

#include "photon/math/vec3.hpp"

#include <vector>

namespace photon::image {

class Film {
public:
    int width = 0;
    int height = 0;
    std::vector<photon::math::Vec3> pixels;
    std::vector<int> counts;

    void resize(int w, int h) {
        width = w;
        height = h;
        pixels.assign(static_cast<std::size_t>(w * h), photon::math::Vec3{});
        counts.assign(static_cast<std::size_t>(w * h), 0);
    }

    void add_sample(int x, int y, const photon::math::Vec3& color) {
        if (x < 0 || y < 0 || x >= width || y >= height) {
            return;
        }
        const std::size_t index = static_cast<std::size_t>(y * width + x);
        pixels[index] += color;
        counts[index] += 1;
    }

    photon::math::Vec3 pixel(int x, int y) const {
        const std::size_t index = static_cast<std::size_t>(y * width + x);
        if (counts[index] == 0) {
            return {};
        }
        return pixels[index] / static_cast<float>(counts[index]);
    }
};

}
