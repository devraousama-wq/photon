#include "photon/accel/bvh.hpp"

#include <algorithm>
#include <array>
#include <limits>

namespace photon::accel {

namespace {

constexpr int kSahBins = 12;

float sah_cost(int count_a, float area_a, int count_b, float area_b, float inv_parent_area) {
    return 1.0f + (count_a * area_a + count_b * area_b) * inv_parent_area;
}

}

BVH::BVH(const std::vector<const photon::geometry::Primitive*>& primitives, int max_leaf) {
    ordered_.reserve(primitives.size());
    for (const auto* primitive : primitives) {
        ordered_.push_back({primitive, primitive->bounds()});
    }
    if (ordered_.empty()) {
        return;
    }
    nodes_.reserve(ordered_.size() * 2);
    build_recursive(0, static_cast<int>(ordered_.size()));
}

int BVH::build_recursive(int start, int end) {
    photon::math::AABB node_bounds = photon::math::AABB::empty();
    for (int i = start; i < end; ++i) {
        node_bounds.expand(ordered_[i].bounds);
    }

    const int count = end - start;
    const int node_index = static_cast<int>(nodes_.size());
    nodes_.push_back({});
    nodes_[node_index].bounds = node_bounds;
    nodes_[node_index].primitive_start = start;
    nodes_[node_index].primitive_count = count;

    if (count <= 4) {
        return node_index;
    }

    photon::math::AABB centroid_bounds = photon::math::AABB::empty();
    for (int i = start; i < end; ++i) {
        centroid_bounds.expand(ordered_[i].bounds.center());
    }
    const photon::math::Vec3 extent = centroid_bounds.extent();
    int axis = 0;
    if (extent.y > extent.x) {
        axis = 1;
    }
    if (extent.z > extent[axis]) {
        axis = 2;
    }

    const float parent_area = node_bounds.surface_area();
    const float inv_parent_area = parent_area > 0.0f ? 1.0f / parent_area : 1.0f;

    float best_cost = static_cast<float>(count);
    int best_axis = axis;
    float best_pos = centroid_bounds.center()[axis];

    std::array<int, kSahBins> count_left{};
    std::array<int, kSahBins> count_right{};
    std::array<float, kSahBins> area_left{};
    std::array<float, kSahBins> area_right{};

    for (int try_axis = 0; try_axis < 3; ++try_axis) {
        if (centroid_bounds.extent()[try_axis] <= 0.0f) {
            continue;
        }
        const float min_c = centroid_bounds.min[try_axis];
        const float max_c = centroid_bounds.max[try_axis];
        const float bin_scale = kSahBins / (max_c - min_c + 1e-6f);

        count_left.fill(0);
        count_right.fill(0);
        area_left.fill(0.0f);
        area_right.fill(0.0f);

        std::array<photon::math::AABB, kSahBins> bins{};
        for (int i = start; i < end; ++i) {
            const float c = ordered_[i].bounds.center()[try_axis];
            int bin = static_cast<int>((c - min_c) * bin_scale);
            bin = std::clamp(bin, 0, kSahBins - 1);
            bins[bin].expand(ordered_[i].bounds);
            count_left[bin]++;
            area_left[bin] += ordered_[i].bounds.surface_area();
        }

        int right_count = count;
        float right_area = parent_area;
        for (int i = kSahBins - 1; i >= 1; --i) {
            right_count -= count_left[i];
            right_area -= area_left[i];
            count_right[i - 1] = right_count;
            area_right[i - 1] = right_area;
        }

        int left_count = 0;
        float left_area = 0.0f;
        for (int i = 0; i < kSahBins - 1; ++i) {
            left_count += count_left[i];
            left_area += area_left[i];
            const float cost = sah_cost(left_count, left_area, count_right[i], area_right[i], inv_parent_area);
            if (cost < best_cost) {
                best_cost = cost;
                best_axis = try_axis;
                best_pos = min_c + (static_cast<float>(i + 1) / kSahBins) * (max_c - min_c);
            }
        }
    }

    auto mid = std::partition(ordered_.begin() + start, ordered_.begin() + end, [&](const BVHPrimitiveRef& ref) {
        return ref.bounds.center()[best_axis] < best_pos;
    });
    int mid_index = static_cast<int>(mid - ordered_.begin());
    if (mid_index <= start || mid_index >= end) {
        mid_index = start + count / 2;
    }

    nodes_[node_index].primitive_count = 0;
    nodes_[node_index].left = build_recursive(start, mid_index);
    nodes_[node_index].right = build_recursive(mid_index, end);
    return node_index;
}

bool BVH::intersect(const photon::math::Ray& ray, float t_min, float t_max, photon::geometry::HitRecord& rec) const {
    if (nodes_.empty()) {
        return false;
    }
    return intersect_node(0, ray, t_min, t_max, rec);
}

bool BVH::intersect_node(int node_index, const photon::math::Ray& ray, float t_min, float t_max, photon::geometry::HitRecord& rec) const {
    const BVHBuildNode& node = nodes_[node_index];
    float t_enter = 0.0f;
    float t_exit = 0.0f;
    if (!node.bounds.hit(ray, t_enter, t_exit)) {
        return false;
    }

    bool hit_any = false;
    if (node.primitive_count > 0) {
        for (int i = 0; i < node.primitive_count; ++i) {
            const auto& ref = ordered_[node.primitive_start + i];
            photon::geometry::HitRecord temp;
            if (ref.primitive->intersect(ray, t_min, t_max, temp)) {
                hit_any = true;
                t_max = temp.t;
                rec = temp;
            }
        }
        return hit_any;
    }

    if (node.left >= 0 && intersect_node(node.left, ray, t_min, t_max, rec)) {
        hit_any = true;
        t_max = rec.t;
    }
    if (node.right >= 0 && intersect_node(node.right, ray, t_min, t_max, rec)) {
        hit_any = true;
    }
    return hit_any;
}

}
