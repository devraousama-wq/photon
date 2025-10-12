#include "photon/scene/scene.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace photon::scene {

namespace {

float read_float(const std::string& text, const std::string& key, float fallback) {
    const std::string token = "\"" + key + "\":";
    const auto pos = text.find(token);
    if (pos == std::string::npos) {
        return fallback;
    }
    return std::stof(text.substr(pos + token.size()));
}

int read_int(const std::string& text, const std::string& key, int fallback) {
    return static_cast<int>(read_float(text, key, static_cast<float>(fallback)));
}

photon::math::Vec3 read_vec3(const std::string& text, const std::string& key, const photon::math::Vec3& fallback) {
    const std::string token = "\"" + key + "\":";
    const auto pos = text.find(token);
    if (pos == std::string::npos) {
        return fallback;
    }
    const auto start = text.find('[', pos);
    const auto end = text.find(']', start);
    if (start == std::string::npos || end == std::string::npos) {
        return fallback;
    }
    std::string slice = text.substr(start + 1, end - start - 1);
    for (char& c : slice) {
        if (c == ',') {
            c = ' ';
        }
    }
    std::istringstream in(slice);
    photon::math::Vec3 out = fallback;
    in >> out.x >> out.y >> out.z;
    return out;
}

}

Scene load_scene_json(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("failed to open scene file");
    }
    std::ostringstream buffer;
    buffer << in.rdbuf();
    const std::string text = buffer.str();

    Scene scene;
    scene.settings.width = read_int(text, "width", scene.settings.width);
    scene.settings.height = read_int(text, "height", scene.settings.height);
    scene.settings.samples = read_int(text, "samples", scene.settings.samples);
    scene.settings.max_depth = read_int(text, "max_depth", scene.settings.max_depth);
    scene.settings.tile_size = read_int(text, "tile_size", scene.settings.tile_size);
    scene.settings.threads = read_int(text, "threads", scene.settings.threads);
    scene.settings.seed = static_cast<unsigned>(read_int(text, "seed", static_cast<int>(scene.settings.seed)));

    scene.camera.origin = read_vec3(text, "origin", scene.camera.origin);
    scene.camera.look_at = read_vec3(text, "look_at", scene.camera.look_at);
    scene.camera.up = read_vec3(text, "up", scene.camera.up);
    scene.camera.vertical_fov = read_float(text, "fov", scene.camera.vertical_fov);
    scene.camera.aspect = static_cast<float>(scene.settings.width) / static_cast<float>(scene.settings.height);

    const int floor_id = scene.materials.add(std::make_unique<photon::materials::Lambertian>(read_vec3(text, "floor_color", {0.8f, 0.8f, 0.8f})));
    scene.materials.add(std::make_unique<photon::materials::Lambertian>(read_vec3(text, "left_color", {0.8f, 0.2f, 0.2f})));
    scene.materials.add(std::make_unique<photon::materials::Lambertian>(read_vec3(text, "right_color", {0.2f, 0.2f, 0.8f})));
    scene.materials.add(std::make_unique<photon::materials::Emissive>(read_vec3(text, "light_radiance", {8.0f, 8.0f, 8.0f})));

    scene.objects.add(std::make_unique<photon::geometry::Sphere>(read_vec3(text, "sphere_center", {0.0f, 1.0f, 0.0f}), read_float(text, "sphere_radius", 1.0f), floor_id));

    auto floor = std::make_unique<photon::geometry::Mesh>();
    floor->material_id = floor_id;
    floor->add_triangle(
        {{ -2.0f, 0.0f, -2.0f}, {0,1,0}, {}},
        {{  2.0f, 0.0f, -2.0f}, {0,1,0}, {}},
        {{  2.0f, 0.0f,  2.0f}, {0,1,0}, {}});
    floor->add_triangle(
        {{ -2.0f, 0.0f, -2.0f}, {0,1,0}, {}},
        {{  2.0f, 0.0f,  2.0f}, {0,1,0}, {}},
        {{ -2.0f, 0.0f,  2.0f}, {0,1,0}, {}});
    scene.objects.add(std::move(floor));

    auto light = std::make_unique<photon::lights::AreaLight>(
        read_vec3(text, "light_corner", {-0.5f, 2.98f, -0.5f}),
        read_vec3(text, "light_u", {1.0f, 0.0f, 0.0f}),
        read_vec3(text, "light_v", {0.0f, 0.0f, 1.0f}),
        read_vec3(text, "light_radiance", {12.0f, 12.0f, 12.0f}));
    scene.lights.add(light.get());
    scene.owned_lights.push_back(std::move(light));

    scene.build_accel();
    return scene;
}

std::vector<std::string> validate_scene_json(const std::string& path) {
    std::vector<std::string> errors;
    try {
        const Scene scene = load_scene_json(path);
        if (scene.settings.width <= 0 || scene.settings.height <= 0) {
            errors.push_back("invalid resolution");
        }
        if (scene.objects.objects.empty()) {
            errors.push_back("scene has no objects");
        }
        if (scene.bvh == nullptr) {
            errors.push_back("bvh not built");
        }
    } catch (const std::exception& ex) {
        errors.push_back(ex.what());
    }
    return errors;
}

}
