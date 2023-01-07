#ifndef VEC3_H
#define VEC3_H 

#include <cmath>
#include "RandomUtil.h"
#include <curand_kernel.h>
#include "Cuda.h"

class vec3 {
public:
	vec3() = default;
	CUDA_SHARED vec3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	CUDA_SHARED inline float len() const { return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }
	CUDA_SHARED inline float lensqr() const { return pow(len(), 2); }
	CUDA_SHARED inline vec3 unit() const { float l = len(); return vec3(x / l, y / l, z / l); }
	CUDA_SHARED inline float dot(const vec3& other) const { return x * other.x + y * other.y + z * other.z; }
	CUDA_SHARED inline vec3 cross(const vec3& other) const 
	{
		return vec3(
			y * other.z + z * other.y,
			z * other.x + x * other.z,
			x * other.y + y * other.x);
	}

	CUDA_SHARED inline vec3 operator-() const { return vec3(-x, -y, -z); }
	CUDA_SHARED inline float operator[](uint8_t i) const { return i ? (i == 1 ? y : z) : x; }
	CUDA_SHARED inline float& operator[](uint8_t i) { return i ? (i == 1 ? y : z) : x; }

	CUDA_SHARED inline vec3 operator+(const vec3& v2) const { return vec3(x + v2.x, y + v2.y, z + v2.z); }
	CUDA_SHARED inline vec3 operator-(const vec3& v2) const { return vec3(x - v2.x, y - v2.y, z - v2.z); }
	CUDA_SHARED inline vec3 operator+(float s) const { return vec3(x + s, y + s, z + s); }
	CUDA_SHARED inline vec3 operator-(float s) const { return vec3(x - s, y - s, z - s); }

	CUDA_SHARED const vec3 operator*(float s) const { return vec3(x * s, y * s, z * s); }
	CUDA_SHARED const vec3 operator/(float s) const { return vec3(x / s, y / s, z / s); }
	CUDA_SHARED inline vec3 operator*(const vec3& v2) const { return vec3(x * v2.x, y * v2.y, z * v2.z); }
	CUDA_SHARED inline vec3 operator/(const vec3& v2) const { return vec3(x / v2.x, y / v2.y, z / v2.z); }

	CUDA_SHARED inline vec3& operator+=(const vec3& other) { x += other.x, y += other.y, z += other.z; return *this; }
	CUDA_SHARED inline vec3& operator-=(const vec3& other) { x -= other.x, y -= other.y, z -= other.z; return *this; }
	CUDA_SHARED inline vec3& operator*=(const vec3& other) { x *= other.x, y *= other.y, z *= other.z; return *this; }
	CUDA_SHARED inline vec3& operator/=(const vec3& other) { x /= other.x, y /= other.y, z /= other.z; return *this; }
	CUDA_SHARED inline vec3& operator*=(const float s) { x *= s, y *= s, z *= s; return *this; }
	CUDA_SHARED inline vec3& operator/=(const float s) { x /= s, y /= s, z /= s; return *this; }

	__device__ inline static vec3 random(curandState* local_rand_state) {
		return vec3(Random::Float(local_rand_state), Random::Float(local_rand_state), Random::Float(local_rand_state));
	}


	__device__ inline static vec3 random(curandState* local_rand_state, float min, float max) {
		return vec3(Random::Float(local_rand_state, min, max), Random::Float(local_rand_state, min, max), Random::Float(local_rand_state, min, max));
	}


	__device__ inline static vec3 random_unit(curandState* local_rand_state)
	{
		return vec3(Random::Float(local_rand_state), Random::Float(local_rand_state), Random::Float(local_rand_state)).unit();
	}

	__device__ static vec3 random_in_unit_sphere(curandState* local_rand_state)
	{
		while (true) {
			auto p = vec3::random(local_rand_state, -1, 1);
			if (p.lensqr() >= 1) continue;
			return p;
		}
	}

	__device__ static vec3 random_in_unit_disk(curandState* local_rand_state)
	{
		vec3 p;
		do {
			p = vec3(curand_uniform(local_rand_state), curand_uniform(local_rand_state), 0) * 2.f - vec3(1, 1, 0);
		} while (p.dot(p) >= 1.0f);
		return p;
	}

	CUDA_SHARED bool near_zero() const
	{
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
	}

	CUDA_SHARED vec3 reflect(const vec3& n)
	{
		return *this - n * 2 * dot(n);
	}

	CUDA_SHARED vec3 refract(const vec3& n, float etai_over_etat)
	{
		auto cos_theta = fmin(-(*this).dot(n), 1.f);
		vec3 r_out_perp = (*this + n * cos_theta) * etai_over_etat;
		vec3 r_out_parallel = n * -sqrt(fabs(1.f - r_out_perp.lensqr()));
		return r_out_perp + r_out_parallel;
	}

public:
	union { float x, r; };
	union { float y, g; };
	union { float z, b; };
};

typedef vec3 color;
typedef vec3 point3;

#endif