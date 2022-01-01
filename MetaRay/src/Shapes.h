#pragma once

#include "Hittable.h" 

class Sphere : public Hittable {
public:
	Sphere(const vec3& center, float radius, std::shared_ptr<class Material> mat);
	bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

private:
	vec3 m_Center;
	float m_Radius;
	std::shared_ptr<Material> m_Mat;
};