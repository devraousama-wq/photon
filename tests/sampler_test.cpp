#include <gtest/gtest.h>

#include "photon/samplers/pcg32.hpp"
#include "photon/samplers/sampler.hpp"

using photon::samplers::PCG32;
using photon::samplers::PixelSampler;

TEST(Sampler, DeterministicPCG) {
    PCG32 a;
    PCG32 b;
    a.seed_state(42, 1);
    b.seed_state(42, 1);
    EXPECT_EQ(a.next_uint(), b.next_uint());
}

TEST(Sampler, StratifiedPixel) {
    PixelSampler sampler{4};
    PCG32 rng;
    rng.seed_state(7, 2);
    const auto sample = sampler.pixel_sample(rng, 0, 64, 64, 10, 20);
    EXPECT_GT(sample.u, 0.0f);
    EXPECT_LT(sample.u, 1.0f);
    EXPECT_GT(sample.v, 0.0f);
    EXPECT_LT(sample.v, 1.0f);
}

TEST(Sampler, PerPixelSeed) {
    PixelSampler sampler{8};
    const PCG32 a = sampler.make_rng(99, 1, 2, 0);
    const PCG32 b = sampler.make_rng(99, 1, 2, 0);
    PCG32 copy_a = a;
    PCG32 copy_b = b;
    EXPECT_FLOAT_EQ(copy_a.next_float(), copy_b.next_float());
}
