#ifndef VEC3_H
#define VEC3_H 

#include <cmath>
#include <curand_kernel.h>
#include "CudaCore.h"

#ifdef CUDA_ENABLED

class vec3 {
public:
	CUDA_SHARED vec3() : x(0.f), y(0.f), z(0.f) {}
	CUDA_SHARED vec3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	CUDA_SHARED inline float len() const { return static_cast<float>(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2))); }
	CUDA_SHARED inline float lensqr() const { return static_cast<float>(pow(len(), 2)); }
	CUDA_SHARED inline vec3 unit() const { float l = len(); return vec3(x / l, y / l, z / l); }
	CUDA_SHARED inline float dot(const vec3& other) const { return x * other.x + y * other.y + z * other.z; }
	CUDA_SHARED inline vec3 cross(const vec3& other) const {
		return vec3(
			y * other.z + z * other.y,
			z * other.x + x * other.z,
			x * other.y + y * other.x);
	}

	CUDA_SHARED inline const vec3& operator+() const { return *this; }
	CUDA_SHARED inline vec3 operator-() const { return vec3(-x, -y, -z); }
	CUDA_SHARED inline float operator[](uint8_t i) const { return i ? (i == 1 ? y : z) : x; }
	CUDA_SHARED inline float& operator[](uint8_t i) { return i ? (i == 1 ? y : z) : x; }

	CUDA_SHARED inline vec3& operator+=(const vec3& other) { x += other.x, y += other.y, z += other.z; return *this; }
	CUDA_SHARED inline vec3& operator-=(const vec3& other) { x -= other.x, y -= other.y, z -= other.z; return *this; }
	CUDA_SHARED inline vec3& operator*=(const vec3& other) { x *= other.x, y *= other.y, z *= other.z; return *this; }
	CUDA_SHARED inline vec3& operator/=(const vec3& other) { x /= other.x, y /= other.y, z /= other.z; return *this; }
	CUDA_SHARED inline vec3& operator*=(const float s) { x *= s, y *= s, z *= s; return *this; }
	CUDA_SHARED inline vec3& operator/=(const float s) { x /= s, y /= s, z /= s; return *this; }

	__device__ inline static vec3 random(curandState* randState) {
		return vec3(curand_uniform(randState), curand_uniform(randState), curand_uniform(randState));
	}

public:
	union { float x, r; };
	union { float y, g; };
	union { float z, b; };
};

inline std::istream& operator>>(std::istream& is, vec3& t) {
	return is >> t.x >> t.y >> t.z;
}

inline std::ostream& operator<<(std::ostream& os, const vec3& t) {
	return os << t.x << " " << t.y << " " << t.z;
}

CUDA_SHARED inline vec3 operator+(const vec3& v1, const vec3& v2) { return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
CUDA_SHARED inline vec3 operator-(const vec3& v1, const vec3& v2) { return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
CUDA_SHARED inline vec3 operator+(float s, const vec3& v) { return vec3(v.x + s, v.y + s, v.z + s); }
CUDA_SHARED inline vec3 operator-(const vec3& v, float s) { return vec3(v.x - s, v.y - s, v.z - s); }

CUDA_SHARED inline vec3 operator*(const vec3& v1, const vec3& v2) { return vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }
CUDA_SHARED inline vec3 operator/(const vec3& v1, const vec3& v2) { return vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z); }
CUDA_SHARED inline vec3 operator*(float s, const vec3& v) { return vec3(v.x * s, v.y * s, v.z * s); }
CUDA_SHARED inline vec3 operator/(const vec3& v, float s) { return vec3(v.x / s, v.y / s, v.z / s); }


#else

#include <glm/glm.hpp>

class vec3 : public glm::vec3 {
public:
	vec3() = default;

	vec3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }

	vec3(const glm::vec3& v) { this->x = v.x; this->y = v.y; this->z = v.z; }

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

#endif

typedef vec3 color;
typedef vec3 point3;

#endif