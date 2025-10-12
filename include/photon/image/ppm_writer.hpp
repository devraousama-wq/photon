#pragma once

#include "photon/image/film.hpp"

#include <fstream>
#include <string>

namespace photon::image {

inline photon::math::Vec3 tonemap_reinhard(const photon::math::Vec3& color) {
    return {
        color.x / (1.0f + color.x),
        color.y / (1.0f + color.y),
        color.z / (1.0f + color.z),
    };
}

inline void write_ppm(const Film& film, const std::string& path) {
    std::ofstream out(path, std::ios::binary);
    out << "P6\n" << film.width << " " << film.height << "\n255\n";
    for (int y = 0; y < film.height; ++y) {
        for (int x = 0; x < film.width; ++x) {
            const auto mapped = tonemap_reinhard(film.pixel(x, y));
            const auto to_byte = [](float v) {
                const float clamped = v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
                return static_cast<unsigned char>(clamped * 255.0f + 0.5f);
            };
            const unsigned char rgb[3] = {to_byte(mapped.x), to_byte(mapped.y), to_byte(mapped.z)};
            out.write(reinterpret_cast<const char*>(rgb), 3);
        }
    }
}

}
