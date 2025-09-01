#pragma once

#include "photon/materials/dielectric.hpp"
#include "photon/materials/ggx.hpp"
#include "photon/materials/lambertian.hpp"
#include "photon/materials/material.hpp"

#include <memory>
#include <vector>

namespace photon::materials {

class MaterialLibrary {
public:
    std::vector<std::unique_ptr<Material>> materials;

    int add(std::unique_ptr<Material> material) {
        materials.push_back(std::move(material));
        return static_cast<int>(materials.size()) - 1;
    }

    const Material* get(int id) const {
        if (id < 0 || id >= static_cast<int>(materials.size())) {
            return nullptr;
        }
        return materials[id].get();
    }
};

}
