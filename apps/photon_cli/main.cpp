#include "photon/config.hpp"
#include "photon/version.hpp"

#include <iostream>
#include <string_view>

namespace {

void print_usage() {
    std::cout << "photon " << photon::kVersion << "\n";
    std::cout << "usage: photon [--health] [--version]\n";
}

}

int main(int argc, char** argv) {
    if (argc == 1) {
        print_usage();
        return 0;
    }

    for (int i = 1; i < argc; ++i) {
        const std::string_view arg = argv[i];
        if (arg == "--health") {
            const auto config = photon::load_config_from_env();
            std::cout << "ok threads=" << config.threads
                      << " tile=" << config.tile_size
                      << " samples=" << config.samples
                      << " depth=" << config.max_depth << "\n";
            return 0;
        }
        if (arg == "--version") {
            std::cout << photon::kVersion << "\n";
            return 0;
        }
        print_usage();
        return 1;
    }

    return 0;
}
