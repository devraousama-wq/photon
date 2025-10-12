#include "photon/config.hpp"
#include "photon/image/film.hpp"
#include "photon/image/ppm_writer.hpp"
#include "photon/renderer/tile_renderer.hpp"
#include "photon/scene/json_loader.hpp"
#include "photon/version.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace {

void print_usage() {
    std::cout << "photon " << photon::kVersion << "\n";
    std::cout << "usage:\n";
    std::cout << "  photon --health\n";
    std::cout << "  photon --version\n";
    std::cout << "  photon validate <scene.json>\n";
    std::cout << "  photon render <scene.json> --output out.ppm\n";
}

int run_validate(const std::string& path) {
    const auto errors = photon::scene::validate_scene_json(path);
    if (errors.empty()) {
        std::cout << "scene ok\n";
        return 0;
    }
    for (const auto& err : errors) {
        std::cerr << err << "\n";
    }
    return 1;
}

int run_render(const std::string& path, const std::string& output) {
    auto scene = photon::scene::load_scene_json(path);
    photon::image::Film film;
    film.resize(scene.settings.width, scene.settings.height);
    photon::renderer::TileRenderer renderer;
    renderer.render(scene, film);
    photon::image::write_ppm(film, output);
    std::cout << "wrote " << output << " (" << scene.settings.width << "x" << scene.settings.height << ")\n";
    return 0;
}

}

int main(int argc, char** argv) {
    if (argc == 1) {
        print_usage();
        return 0;
    }

    const std::string_view cmd = argv[1];
    if (cmd == "--health") {
        const auto config = photon::load_config_from_env();
        std::cout << "ok threads=" << config.threads
                  << " tile=" << config.tile_size
                  << " samples=" << config.samples
                  << " depth=" << config.max_depth << "\n";
        return 0;
    }
    if (cmd == "--version") {
        std::cout << photon::kVersion << "\n";
        return 0;
    }
    if (cmd == "validate") {
        if (argc < 3) {
            print_usage();
            return 1;
        }
        return run_validate(argv[2]);
    }
    if (cmd == "render") {
        if (argc < 3) {
            print_usage();
            return 1;
        }
        std::string output = "out.ppm";
        for (int i = 3; i + 1 < argc; ++i) {
            if (std::string_view(argv[i]) == "--output") {
                output = argv[i + 1];
            }
        }
        return run_render(argv[2], output);
    }

    print_usage();
    return 1;
}
