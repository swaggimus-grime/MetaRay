#ifndef CAMERA_H
#define CAMERA_H 

#include "Ray.h"

class Camera {
public:
    Camera(point3 lookfrom, point3 lookat, vec3 vup,
        float vfov, float aspectRatio, float aperture, float focusDist) 
    {
        m_LensRadius = aperture / 2;
        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspectRatio * half_height;
        m_Origin = lookfrom;
        m_W = static_cast<point3>(lookfrom - lookat).unit();
        m_U = static_cast<point3>(vup.cross(m_W)).unit();
        m_V = m_W.cross(m_U);
        m_LowerLeftCorner = m_Origin - half_width * focusDist * m_U - half_height * focusDist * m_V - focusDist * m_W;
        m_Horizontal = 2 * half_width * focusDist * m_U;
        m_Vertical = 2 * half_height * focusDist * m_V;
    }

    ray LookAt(float s, float t) const {
        vec3 rd = m_LensRadius * vec3::random_in_unit_disk();
        vec3 offset = m_U * rd.x + m_V * rd.y;

        return ray{
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

#endif
