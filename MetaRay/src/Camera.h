#pragma once

#include "Ray.h"

class Camera {
public:
    Camera(const vec3& origin, float aspectRatio) {
        float viewportHeight = 2.f;
        float viewportWidth = aspectRatio * viewportHeight;
        float focal_length = 1.0;

        m_Origin = vec3(0.f, 0.f, 0.f);
        m_Horizontal = vec3(viewportWidth, 0.f, 0.f);
        m_Vertical = vec3(0.f, viewportHeight, 0.f);
        m_LowerLeftCorner = m_Origin - m_Horizontal / 2.f - m_Vertical / 2.f - vec3(0, 0, focal_length);
    }

    ray GetRay(float u, float v) const {
        return ray{ m_Origin, m_LowerLeftCorner + u * m_Horizontal + v * m_Vertical - m_Origin };
    }

private:
    vec3 m_Origin;
    vec3 m_LowerLeftCorner;
    vec3 m_Horizontal;
    vec3 m_Vertical;
};