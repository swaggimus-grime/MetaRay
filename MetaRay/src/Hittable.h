#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

struct hit_record {
    glm::vec3 p;
    glm::vec3 normal;
    double t;
};

class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif