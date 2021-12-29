#pragma once

#include "Ray.h"

struct hit_record {
    glm::vec3 p;
    glm::vec3 normal;
    double t;
};

class Hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};