#pragma once

#include "photon/image/film.hpp"
#include "photon/integrators/path_tracer.hpp"
#include "photon/scene/scene.hpp"
#include "photon/samplers/sampler.hpp"

#include <atomic>
#include <stop_token>
#include <thread>
#include <vector>

namespace photon::renderer {

struct Tile {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
};

class TileRenderer {
public:
    void render(photon::scene::Scene& scene, photon::image::Film& film, const std::stop_token& stop_token = {}) const {
        const int tile = scene.settings.tile_size;
        std::vector<Tile> tiles;
        for (int y = 0; y < scene.settings.height; y += tile) {
            for (int x = 0; x < scene.settings.width; x += tile) {
                tiles.push_back({
                    x,
                    y,
                    std::min(tile, scene.settings.width - x),
                    std::min(tile, scene.settings.height - y),
                });
            }
        }

        std::atomic<std::size_t> next{0};
        const photon::integrators::PathTracer integrator = scene.make_integrator();
        const photon::samplers::PixelSampler sampler(scene.settings.samples);

        const int worker_count = std::max(1, scene.settings.threads);
        std::vector<std::jthread> workers;
        workers.reserve(static_cast<std::size_t>(worker_count));

        for (int i = 0; i < worker_count; ++i) {
            workers.emplace_back([&, stop_token](std::stop_token token) {
                while (!token.stop_requested()) {
                    const std::size_t index = next.fetch_add(1);
                    if (index >= tiles.size()) {
                        break;
                    }
                    const Tile work = tiles[index];
                    for (int py = 0; py < work.height; ++py) {
                        for (int px = 0; px < work.width; ++px) {
                            const int x = work.x + px;
                            const int y = work.y + py;
                            photon::math::Vec3 color{};
                            for (int s = 0; s < sampler.samples_per_pixel(); ++s) {
                                auto rng = sampler.make_rng(scene.settings.seed, x, y, s);
                                const auto sample = sampler.pixel_sample(rng, s, scene.settings.width, scene.settings.height, x, y);
                                const auto ray = scene.camera.generate_ray(sample, rng);
                                color += integrator.trace(ray, rng);
                            }
                            color /= static_cast<float>(sampler.samples_per_pixel());
                            film.add_sample(x, y, color);
                        }
                    }
                }
            }, stop_token);
        }
    }
};

}
