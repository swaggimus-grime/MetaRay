#ifndef RAY_H
#define RAY_H 

#include "Vec3.h"

struct ray {
	vec3 origin;
	vec3 direction;

    ray() = default;
	
	CUDA_SHARED ray(const vec3& origin, const vec3& direction)
		: origin(origin), direction(direction)
	{}

	
	CUDA_SHARED vec3 at(float t) const {
		return origin + t * direction;
	}
};

#endif