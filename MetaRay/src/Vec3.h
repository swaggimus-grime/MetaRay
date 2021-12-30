#pragma once

#include <glm/glm.hpp>
#include <cmath>

extern float random_float();
extern float random_float(float min, float max);

class vec3 : public glm::vec3 {
public:
	vec3() = default;
	vec3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	vec3(const glm::vec3& v) { this->x = v.x; this->y = v.y; this->z = v.z;}
	operator glm::vec3()& { return static_cast<glm::vec3&>(*this); }
	inline float len() const { return glm::length(static_cast<glm::vec3>(*this)); }
	inline float lensqr() const { return (float)pow(len(), 2); }
	inline float dot(const vec3& other) const { return glm::dot(static_cast<glm::vec3>(*this), other); }

	inline static vec3 random() {
		return vec3(random_float(), random_float(), random_float());
	}

	inline static vec3 random(float min, float max) {
		return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
	}
};

typedef vec3 color;
typedef vec3 point3;