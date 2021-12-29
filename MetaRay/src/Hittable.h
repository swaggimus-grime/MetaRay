#pragma once

#include "Ray.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "Vec3.h"

struct hit_record {
    vec3 p;
    vec3 normal;
    double t;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = r.direction.dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

class HitList : public Hittable {
public:
    HitList() {}

    inline void add(std::shared_ptr<Hittable> object) { m_Objects.push_back(object); }
    inline void clear() { m_Objects.clear(); }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    std::vector<std::shared_ptr<Hittable>> m_Objects;
};
