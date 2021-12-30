#include "Shapes.h"
#include "Vec3.h"

Sphere::Sphere(const vec3& center, float radius)
	:m_Center(center), m_Radius(radius)
{
}

bool Sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    vec3 oc = r.origin - m_Center;
    auto a = r.direction.lensqr();
    float half_b = oc.dot(r.direction);
    float c = oc.lensqr() - pow(m_Radius, 2);

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - m_Center) / m_Radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}
