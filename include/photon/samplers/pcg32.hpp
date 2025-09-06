#pragma once

#include <cstdint>

namespace photon::samplers {

class PCG32 {
public:
    PCG32() = default;
    PCG32(uint64_t seed, uint64_t seq = 1) { seed_state(seed, seq); }

    void seed_state(uint64_t seed, uint64_t seq) {
        state_ = 0;
        inc_ = (seq << 1u) | 1u;
        next_uint();
        state_ += seed;
        next_uint();
    }

    uint32_t next_uint() {
        const uint64_t old = state_;
        state_ = old * 6364136223846793005ULL + inc_;
        const uint32_t xorshifted = static_cast<uint32_t>(((old >> 18u) ^ old) >> 27u);
        const uint32_t rot = static_cast<uint32_t>(old >> 59u);
        return (xorshifted >> rot) | (xorshifted << ((-static_cast<int32_t>(rot)) & 31));
    }

    float next_float() {
        return (next_uint() >> 8) * 0x1.0p-24f;
    }

    float next_float(float min_v, float max_v) {
        return min_v + (max_v - min_v) * next_float();
    }

private:
    uint64_t state_ = 0x853c49e6748fea9bULL;
    uint64_t inc_ = 0xda3e39cb94b95bdbULL;
};

inline float stratified1d(PCG32& rng, int index, int count) {
    const float stratum = (static_cast<float>(index) + rng.next_float()) / static_cast<float>(count);
    return stratum;
}

}
