#pragma once

#include "Vec3.h"
#include <glm/glm.hpp>

struct ray {
	vec3 origin;
	vec3 direction;

	vec3 at(float t) const {
		return origin + t * direction;
	}
};