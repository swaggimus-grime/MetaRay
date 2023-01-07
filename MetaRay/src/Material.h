#ifndef MATERIAL_H
#define MATERIAL_H 

#include "Hittable.h"

class Material {
public:
    __device__ virtual bool Scatter (const ray& r_in, const hit_record& rec,
        color& attenuation, ray& scattered, curandState* local_rand_state) const = 0;
};

class Diffuse : public Material {
public:
    __device__ Diffuse(const color& a) : m_Albedo(a) {}

    __device__ virtual bool Scatter(const ray& r_in, const hit_record& rec, 
        color& attenuation, ray& scattered, curandState* local_rand_state) const override {
        vec3 scatter_direction = rec.normal + vec3::random_unit(local_rand_state);

        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray{ rec.p, scatter_direction };
        attenuation = m_Albedo;
        return true;
    }

public:
    color m_Albedo;
};

class Metal : public Material {
public:
    __device__ Metal(const color& a, float fuzz) : m_Albedo(a), m_Fuzz(fuzz < 1 ? fuzz : 1) {}

    __device__ virtual bool Scatter (const ray& r_in, const hit_record& rec, 
        color& attenuation, ray& scattered, curandState* local_rand_state) const override {
        vec3 reflected = r_in.direction.unit().reflect(rec.normal);
        scattered = ray{ rec.p, reflected +  vec3::random_in_unit_sphere(local_rand_state) * m_Fuzz };
        attenuation = m_Albedo;
        return scattered.direction.dot(rec.normal) > 0;
    }

public:
    color m_Albedo;
    float m_Fuzz;
};

class Dielectric : public Material {
public:
    __device__ Dielectric(float index_of_refraction) : ir(index_of_refraction) {}

    __device__ virtual bool Scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, curandState*  local_rand_state
    ) const override {
        attenuation = color(1.0, 1.0, 1.0);
        float refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        vec3 unit_direction = r_in.direction.unit();
        double cos_theta = -unit_direction.dot(rec.normal);
        cos_theta = cos_theta < 1.0 ? cos_theta : 1.0;
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract || Reflectance(cos_theta, refraction_ratio) > Random::Float(local_rand_state))
            direction = unit_direction.reflect(rec.normal);
        else
            direction = unit_direction.refract(rec.normal, refraction_ratio);

        scattered = ray{ rec.p, direction };
        return true;
    }

private:
    __device__ float Reflectance(float cosine, float ref_idx) const {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
public:
    float ir; // Index of Refraction
};

#endif