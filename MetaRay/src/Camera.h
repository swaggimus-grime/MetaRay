#pragma once

#include "Ray.h"

class Camera {
public:
    Camera(point3 lookfrom, point3 lookat, vec3 vup, 
        float vfov, float aspectRatio, float aperture, float focusDist) {
        float theta = vfov;
        float h = tan(theta / 2);
        float viewportHeight = 2.f;
        float viewportWidth = aspectRatio * viewportHeight;
        float focal_length = 1.0;

        m_W = static_cast<point3>(lookfrom - lookat).unit();
        m_U = static_cast<point3>(cross(vup, m_W)).unit();
        m_V = cross(m_W, m_U);
        m_Origin = lookfrom;
        m_Horizontal = focusDist * viewportWidth * m_U;
        m_Vertical = focusDist * viewportHeight * m_V;
        m_LowerLeftCorner = m_Origin - m_Horizontal / 2.f - m_Vertical / 2.f - m_W;

        m_LensRadius = aperture / 2.f;
    }

    ray LookAt(float s, float t) const {
        vec3 rd = m_LensRadius * vec3::random_in_unit_disk();
        vec3 offset = m_U * rd.x + m_V * rd.y;

        return ray {
            m_Origin + offset,
            m_LowerLeftCorner + s * m_Horizontal + t * m_Vertical - m_Origin - offset
        };
    }

private:
    vec3 m_Origin;
    vec3 m_LowerLeftCorner;
    vec3 m_Horizontal;
    vec3 m_Vertical;

    vec3 m_U, m_V, m_W;
    float m_LensRadius;
};