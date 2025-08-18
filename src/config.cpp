#include "photon/config.hpp"

#include <cstdlib>
#include <stdexcept>

namespace photon {

namespace {

int read_int(const char* name, int fallback) {
    const char* value = std::getenv(name);
    if (value == nullptr || *value == '\0') {
        return fallback;
    }
    return std::stoi(value);
}

}

RenderConfig load_config_from_env() {
    RenderConfig config;
    config.threads = read_int("PHOTON_THREADS", config.threads);
    config.tile_size = read_int("PHOTON_TILE_SIZE", config.tile_size);
    config.samples = read_int("PHOTON_DEFAULT_SAMPLES", config.samples);
    config.max_depth = read_int("PHOTON_DEFAULT_MAX_DEPTH", config.max_depth);
    if (config.threads < 1 || config.tile_size < 1 || config.samples < 1 || config.max_depth < 1) {
        throw std::runtime_error("invalid photon configuration");
    }
    return config;
}

}
