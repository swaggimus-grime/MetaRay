#pragma once

#include <glm/glm.hpp>
#include <cmath>
#include "RandomUtil.h"

class vec3 : public glm::vec3 {
public:
	vec3() = default;
	vec3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	vec3(const glm::vec3& v) { this->x = v.x; this->y = v.y; this->z = v.z;}
	operator glm::vec3()& { return static_cast<glm::vec3&>(*this); }
	inline float len() const { return glm::length(static_cast<glm::vec3>(*this)); }
	inline float lensqr() const { return (float)pow(len(), 2); }
	inline float dot(const vec3& other) const { return glm::dot(static_cast<glm::vec3>(*this), other); }
	inline vec3 cross(const vec3& other) const { return glm::cross(*this, other); }
	inline vec3 unit() const { return glm::normalize(*this); }

	inline static vec3 random() {
		return vec3(Random::Float(), Random::Float(), Random::Float());
	}

	inline static vec3 random(float min, float max) {
		return vec3(Random::Float(min, max), Random::Float(min, max), Random::Float(min, max));
	}

	inline static vec3 random_unit() {
		return glm::normalize(vec3(Random::Float(), Random::Float(), Random::Float()));
	}

	static vec3 random_in_unit_sphere() {
		while (true) {
			auto p = vec3::random(-1, 1);
			if (p.lensqr() >= 1) continue;
			return p;
		}
	}

	static vec3 random_in_unit_disk() {
		while (true) {
			auto p = vec3(Random::Float(-1, 1), Random::Float(-1, 1), 0);
			if (p.lensqr() >= 1) continue;
			return p;
		}
	}

	bool near_zero() const {
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
	}

	vec3 reflect(const vec3& n) {
		return *this - 2 * dot(n) * n;
	}

	vec3 refract(const vec3& n, float etai_over_etat) {
		auto cos_theta = fmin(-(*this).dot(n), 1.f);
		vec3 r_out_perp = etai_over_etat * (*this + cos_theta * n);
		vec3 r_out_parallel = -sqrt(fabs(1.f - r_out_perp.lensqr())) * n;
		return r_out_perp + r_out_parallel;
	}
};

typedef vec3 color;
typedef vec3 point3;