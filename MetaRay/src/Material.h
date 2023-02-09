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
    __device__ Diffuse(const color& a);

    __device__ virtual bool Scatter(const ray& r_in, const hit_record& rec,
        color& attenuation, ray& scattered, curandState* local_rand_state) const override;

public:
    color m_Albedo;
};

class Metal : public Material {
public:
    __device__ Metal(const color& a, float fuzz);

    __device__ virtual bool Scatter(const ray& r_in, const hit_record& rec,
        color& attenuation, ray& scattered, curandState* local_rand_state) const override;

public:
    color m_Albedo;
    float m_Fuzz;
};

class Dielectric : public Material {
public:
    __device__ Dielectric(float index_of_refraction);

    __device__ virtual bool Scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered, curandState* local_rand_state
    ) const override;

private:
    __device__ float Reflectance(float cosine, float ref_idx) const;

public:
    float ir; // Index of Refraction
};

#endif