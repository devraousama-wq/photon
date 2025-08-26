#pragma once

#include "photon/geometry/primitive.hpp"
#include "photon/geometry/triangle.hpp"

#include <memory>
#include <vector>

namespace photon::geometry {

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 uv;
};

class Mesh : public Primitive {
public:
    std::vector<Vertex> vertices;
    std::vector<std::array<int, 3>> indices;
    int material_id = 0;

    void add_triangle(const Vertex& a, const Vertex& b, const Vertex& c) {
        const int base = static_cast<int>(vertices.size());
        vertices.push_back(a);
        vertices.push_back(b);
        vertices.push_back(c);
        indices.push_back({base, base + 1, base + 2});
    }

    bool intersect(const math::Ray& ray, float t_min, float t_max, HitRecord& rec) const override {
        bool hit_any = false;
        float closest = t_max;
        for (const auto& tri_idx : indices) {
            Triangle tri;
            tri.v0 = vertices[tri_idx[0]].position;
            tri.v1 = vertices[tri_idx[1]].position;
            tri.v2 = vertices[tri_idx[2]].position;
            tri.n0 = vertices[tri_idx[0]].normal;
            tri.n1 = vertices[tri_idx[1]].normal;
            tri.n2 = vertices[tri_idx[2]].normal;
            tri.uv0 = vertices[tri_idx[0]].uv;
            tri.uv1 = vertices[tri_idx[1]].uv;
            tri.uv2 = vertices[tri_idx[2]].uv;
            tri.material_id = material_id;
            HitRecord temp;
            if (tri.intersect(ray, t_min, closest, temp)) {
                hit_any = true;
                closest = temp.t;
                rec = temp;
            }
        }
        return hit_any;
    }

    math::AABB bounds() const override {
        math::AABB box = math::AABB::empty();
        for (const auto& v : vertices) {
            box.expand(v.position);
        }
        return box;
    }
};

class SceneObjects {
public:
    std::vector<std::unique_ptr<Primitive>> objects;

    void add(std::unique_ptr<Primitive> primitive) {
        objects.push_back(std::move(primitive));
    }

    bool intersect(const math::Ray& ray, float t_min, float t_max, HitRecord& rec) const {
        bool hit_any = false;
        float closest = t_max;
        for (const auto& obj : objects) {
            HitRecord temp;
            if (obj->intersect(ray, t_min, closest, temp)) {
                hit_any = true;
                closest = temp.t;
                rec = temp;
            }
        }
        return hit_any;
    }

    math::AABB bounds() const {
        math::AABB box = math::AABB::empty();
        for (const auto& obj : objects) {
            box.expand(obj->bounds());
        }
        return box;
    }
};

}
