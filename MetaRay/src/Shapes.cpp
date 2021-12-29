#include "Shapes.h"
#include "vec3util.h"

using glm::vec3;

Sphere::Sphere(const glm::vec3& center, double radius)
	:m_Center(center), m_Radius(radius)
{
}

bool Sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const
{
    vec3 oc = r.origin - m_Center;
    auto a = lensqr(r.direction);
    auto half_b = glm::dot(oc, r.direction);
    auto c = lensqr(oc) - pow(m_Radius, 2);

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
    rec.normal = (rec.p - m_Center) / m_Radius;

    return true;
}
