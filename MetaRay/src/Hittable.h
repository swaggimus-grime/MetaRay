#ifndef HITTABLE_H
#define HITTABLE_H 

#include "Ray.h"

struct hit_record {
    vec3 p;
    vec3 normal;
    float t;
    bool front_face;
#ifndef CUDA_ENABLED
    std::shared_ptr<class Material> mat;
#endif
};

class Hittable {
public:
    CUDA_SHARED virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#ifdef CUDA_ENABLED

class HitList : public Hittable {
public:
    CUDA_SHARED HitList() : m_Objects(nullptr), m_Size(0) {}
    CUDA_SHARED HitList(Hittable** list, int size) { m_Objects = list; m_Size = size; }
    CUDA_SHARED virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const 
    {
        hit_record temp_rec;
        bool hit_anything = false;
        float closest_so_far = tmax;
        for (int i = 0; i < m_Size; i++) {
            if (m_Objects[i]->hit(r, tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }

private:
    Hittable** m_Objects;
    int m_Size;
};

#else

#include <memory>
#include <vector>

class HitList : public Hittable {
public:
    __device__ HitList() = default;

    __device__ void add(std::shared_ptr<Hittable> object) { m_Objects.push_back(object); }
    __device__ inline void clear() { m_Objects.clear(); }

    __device__ virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

private:
    std::vector<std::shared_ptr<Hittable>> m_Objects;
};

#endif

#endif