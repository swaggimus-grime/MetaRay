#pragma once

#include "Hittable.h"

class Sphere : public Hittable {
	Sphere(const glm::vec3& center, double radius);
	bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;

private:
	glm::vec3 m_Center;
	double m_Radius;
};