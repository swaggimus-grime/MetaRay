#ifndef SHAPES_H
#define SHAPES_H 

#include "Hittable.h" 

#ifdef CUDA_ENABLED

class Sphere : public Hittable {
public:
	__device__ Sphere(const vec3& center, float radius);
	__device__ virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

private:
	vec3 m_Center;
	float m_Radius;
};

#else

class Sphere : public Hittable {
public:
	__device__ Sphere(const vec3& center = { 0.f, 0.f, 0.f }, float radius = 0.5f, std::shared_ptr<class Material> mat = nullptr);
	__device__ bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

private:
	vec3 m_Center;
	float m_Radius;
	std::shared_ptr<Material> m_Mat;
};

#endif

#endif