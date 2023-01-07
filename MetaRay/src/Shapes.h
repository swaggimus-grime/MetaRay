#ifndef SHAPES_H
#define SHAPES_H 

#include "Hittable.h" 
#include "Vec3.h"
#include <memory>

class Sphere : public Hittable {
public:
	__device__ Sphere(const point3& center = { 0.f, 0.f, 0.f }, float radius = 0.5f, class Material* mat = nullptr);
	__device__ virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

	Material* m_Mat;
private:
	point3 m_Center;
	float m_Radius;
};

#endif