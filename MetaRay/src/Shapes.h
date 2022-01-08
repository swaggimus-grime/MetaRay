#ifndef SHAPES_H
#define SHAPES_H 

#include "Hittable.h" 

#ifdef CUDA_ENABLED

class Sphere : public Hittable {
public:
	CUDA_SHARED Sphere(const vec3& center, float radius) { m_Center = center; m_Radius = radius; }
	CUDA_SHARED virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const
    {
        vec3 oc = r.origin - m_Center;
        float a = r.direction.dot(r.direction);
        float b = oc.dot(r.direction);
        float c = oc.dot(oc) - m_Radius * m_Radius;
        float discriminant = b * b - a * c;
        if (discriminant > 0) {
            float temp = (-b - sqrt(discriminant)) / a;
            if (temp < t_max && temp > t_min) {
                rec.t = temp;
                rec.p = r.at(rec.t);
                rec.normal = (rec.p - m_Center) / m_Radius;
                return true;
            }
            temp = (-b + sqrt(discriminant)) / a;
            if (temp < t_max && temp > t_min) {
                rec.t = temp;
                rec.p = r.at(rec.t);
                rec.normal = (rec.p - m_Center) / m_Radius;
                return true;
            }
        }
        return false;
    }

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