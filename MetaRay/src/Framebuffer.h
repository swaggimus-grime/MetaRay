#pragma once

#include "Vec3.h"

class Framebuffer {
public:
	Framebuffer(uint32_t width, uint32_t height);
	Framebuffer(uint32_t width, uint32_t height, vec3* data);
	~Framebuffer();
	void Write(vec3* data);
	void Present() const;

	CUDA_SHARED inline void SetPixel(int i, const vec3& c) { m_Data[i] = c; }
	CUDA_SHARED vec3 operator [](int i) const { return m_Data[i]; }
	CUDA_SHARED vec3& operator [](int i) { return m_Data[i]; }

	vec3* m_Data;
private:
	uint32_t m_Width, m_Height;
};