#ifndef HITTABLE_H
#define HITTABLE_H 

#include "Ray.h"
#include <vector>
#include "Vec3.h"

struct hit_record {
    vec3 p;
    vec3 normal;
    float t;
    bool front_face;
    class Material* mat;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = r.direction.dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    __device__ virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

class HitList : public Hittable {
public:
    __device__ HitList(Hittable** l, int n)
        :list(l), size(n)
    {}
    __device__ virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

public:
    Hittable** list;
    int size;
};

#endif