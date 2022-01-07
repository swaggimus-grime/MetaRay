#include "Hittable.h"

#ifdef CUDA_ENABLED

__device__ bool HitList::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;
    for (int i = 0; i < m_Size; i++) {
        if (m_Objects[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

#else

__device__ bool HitList::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : m_Objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif