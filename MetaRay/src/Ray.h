#ifndef RAY_H
#define RAY_H 

#include "Vec3.h"

struct ray {
	vec3 origin;
	vec3 direction;

	__device__ ray() = default;

	__device__ ray(const vec3& origin, const vec3& direction)
		: origin(origin), direction(direction)
	{}

	__device__ vec3 at(float t) const {
		return origin + direction * t;
	}
};

#endif