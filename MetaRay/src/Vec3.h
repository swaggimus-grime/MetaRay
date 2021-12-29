#pragma once

#include <glm/glm.hpp>
#include <cmath>

class vec3 : public glm::vec3 {
public:
	vec3() = default;
	vec3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	vec3(const glm::vec3& v) { this->x = v.x; this->y = v.y; this->z = v.z;}
	operator glm::vec3()& { return static_cast<glm::vec3&>(*this); }
	inline float len() const { return glm::length(static_cast<glm::vec3>(*this)); }
	inline float lensqr() const { return pow(len(), 2); }
	inline float dot(const vec3& other) const { return glm::dot(static_cast<glm::vec3>(*this), other); }

};

