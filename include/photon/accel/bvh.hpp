#pragma once

#include "photon/geometry/primitive.hpp"
#include "photon/math/aabb.hpp"

#include <cstdint>
#include <vector>

namespace photon::accel {

struct BVHPrimitiveRef {
    const photon::geometry::Primitive* primitive = nullptr;
    photon::math::AABB bounds;
};

struct BVHBuildNode {
    photon::math::AABB bounds;
    int left = -1;
    int right = -1;
    int primitive_start = 0;
    int primitive_count = 0;
};

class BVH {
public:
    explicit BVH(const std::vector<const photon::geometry::Primitive*>& primitives, int max_leaf = 4);

    bool intersect(const photon::math::Ray& ray, float t_min, float t_max, photon::geometry::HitRecord& rec) const;

private:
    std::vector<BVHBuildNode> nodes_;
    std::vector<BVHPrimitiveRef> ordered_;

    int build_recursive(int start, int end);
    bool intersect_node(int node_index, const photon::math::Ray& ray, float t_min, float t_max, photon::geometry::HitRecord& rec) const;
};

}
