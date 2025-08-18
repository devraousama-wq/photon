#include "photon/config.hpp"

int main() {
    const auto config = photon::load_config_from_env();
    return config.threads > 0 ? 0 : 1;
}
