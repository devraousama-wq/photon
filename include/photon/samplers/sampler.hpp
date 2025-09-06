#pragma once

#include "photon/samplers/pcg32.hpp"

#include <cmath>

namespace photon::samplers {

struct Sample2D {
    float u = 0.0f;
    float v = 0.0f;
};

class PixelSampler {
public:
    explicit PixelSampler(int samples_per_pixel = 16) : spp_(samples_per_pixel) {}

    int samples_per_pixel() const { return spp_; }

    Sample2D pixel_sample(PCG32& rng, int sample_index, int width, int height, int x, int y) const {
        const int nx = std::max(1, static_cast<int>(std::sqrt(static_cast<float>(spp_))));
        const int ny = (spp_ + nx - 1) / nx;
        const int sx = sample_index % nx;
        const int sy = sample_index / nx;
        const float u = (static_cast<float>(x) + stratified1d(rng, sx, nx)) / static_cast<float>(width);
        const float v = (static_cast<float>(y) + stratified1d(rng, sy, ny)) / static_cast<float>(height);
        return {u, v};
    }

    PCG32 make_rng(unsigned seed, int x, int y, int sample) const {
        PCG32 rng;
        const uint64_t mixed = static_cast<uint64_t>(seed) ^ (static_cast<uint64_t>(x) << 16) ^
                               (static_cast<uint64_t>(y) << 1) ^ static_cast<uint64_t>(sample);
        rng.seed_state(mixed, 0x9e3779b97f4a7c15ULL);
        return rng;
    }

private:
    int spp_;
};

}
