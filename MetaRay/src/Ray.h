#ifndef RAY_H
#define RAY_H 

#include "Vec3.h"
#include <glm/glm.hpp>

struct ray {
	vec3 origin;
	vec3 direction;

	ray() = default;

	ray(const vec3& origin, const vec3& direction)
		: origin(origin), direction(direction)
	{}

	vec3 at(float t) const {
		return origin + t * direction;
	}
};

#endif