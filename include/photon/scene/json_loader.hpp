#pragma once

#include "photon/scene/scene.hpp"

#include <string>
#include <vector>

namespace photon::scene {

Scene load_scene_json(const std::string& path);
std::vector<std::string> validate_scene_json(const std::string& path);

}
