#include "Shapes.h"

#ifdef CUDA_ENABLED

__device__ Sphere::Sphere(const vec3& center, float radius)
    :m_Center(center), m_Radius(radius)
{
}

__device__ bool Sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
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
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            rec.normal = (rec.p - m_Center) / m_Radius;
            return true;
        }
    }
    return false;
}

#else

__device__ Sphere::Sphere(const vec3& center, float radius, std::shared_ptr<Material> mat)
	:m_Center(center), m_Radius(radius), m_Mat(mat)
{
}

__device__ bool Sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
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
    rec.mat = m_Mat;

    return true;
}

#endif