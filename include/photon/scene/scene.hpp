#pragma once

#include "photon/accel/bvh.hpp"
#include "photon/camera/camera.hpp"
#include "photon/geometry/mesh.hpp"
#include "photon/geometry/sphere.hpp"
#include "photon/integrators/path_tracer.hpp"
#include "photon/lights/area_light.hpp"
#include "photon/materials/dielectric.hpp"
#include "photon/materials/ggx.hpp"
#include "photon/materials/lambertian.hpp"
#include "photon/materials/library.hpp"

#include <memory>
#include <string>
#include <vector>

namespace photon::scene {

struct RenderSettings {
    int width = 512;
    int height = 512;
    int samples = 64;
    int max_depth = 8;
    int tile_size = 32;
    int threads = 8;
    unsigned seed = 1;
};

struct Scene {
    photon::camera::Camera camera;
    photon::materials::MaterialLibrary materials;
    photon::geometry::SceneObjects objects;
    photon::lights::LightSampler lights;
    std::vector<std::unique_ptr<photon::lights::Light>> owned_lights;
    RenderSettings settings;
    std::unique_ptr<photon::accel::BVH> bvh;

    void build_accel() {
        std::vector<const photon::geometry::Primitive*> refs;
        refs.reserve(objects.objects.size());
        for (const auto& obj : objects.objects) {
            refs.push_back(obj.get());
        }
        bvh = std::make_unique<photon::accel::BVH>(refs);
    }

    photon::integrators::PathTracer make_integrator() const {
        photon::integrators::PathTracer tracer;
        tracer.bvh = bvh.get();
        tracer.materials = &materials;
        tracer.lights = &lights;
        tracer.max_depth = settings.max_depth;
        return tracer;
    }
};

}
