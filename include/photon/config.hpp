#pragma once

#include <cstddef>
#include <string>

namespace photon {

struct RenderConfig {
    int threads = 8;
    int tile_size = 32;
    int samples = 64;
    int max_depth = 8;
    unsigned seed = 1;
};

RenderConfig load_config_from_env();

}
