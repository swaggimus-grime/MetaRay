#include "Shapes.h"

Sphere::Sphere(const point3& center, float radius, Material* mat)
	:m_Center(center), m_Radius(radius), m_Mat(mat)
{
}

bool Sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    vec3 oc = r.origin - m_Center;
    float a = r.direction.dot(r.direction);
    float b = oc.dot(r.direction);
    float c = oc.dot(oc) - m_Radius * m_Radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - m_Center) / m_Radius;
            rec.mat = m_Mat;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - m_Center) / m_Radius;
            rec.mat = m_Mat;
            return true;
        }
    }
    return false;
}
