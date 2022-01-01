#include <iostream>
#include "Ray.h"
#include <limits>
#include "Vec3.h"
#include "Hittable.h"
#include "Shapes.h"
#include "Camera.h"
#include <random>
#include "Material.h"

#define SAMPLES_PER_PIXEL 4
#define INF std::numeric_limits<float>::infinity()

inline float random_float() {
    static std::uniform_real_distribution<float> distribution(0.f, 1.f);
    static std::mt19937 generator;
    return distribution(generator);
}

inline float random_float(float min, float max) {
    static std::uniform_real_distribution<float> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}

float hit_sphere(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.origin - center;
    auto a = r.direction.len();
    float half_b = oc.dot(r.direction);
    auto c = oc.lensqr() - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = vec3::random_in_unit_sphere();
    if (in_unit_sphere.dot(normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

color ray_color(const ray& r, const Hittable& hitObj, unsigned int depth) {
    if (depth <= 0)
        return color(0, 0, 0);

    hit_record rec;
    if (hitObj.hit(r, 0.001f, INF, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat->Scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, hitObj, depth - 1);
        return color(0, 0, 0);
    }
    vec3 unit_direction = glm::normalize(r.direction);
    float t = 0.5f * (unit_direction.y + 1.f);
    return (1.f - t) * vec3(1.f, 1.f, 1.f) + t * vec3(0.5f, 0.7f, 1.f);
}

void write_color(std::ostream& out, vec3 pixel_color) {
    auto r = pixel_color.x;
    auto g = pixel_color.y;
    auto b = pixel_color.z;

    // Divide the color by the number of samples.
    auto scale = 1.f / SAMPLES_PER_PIXEL;
    r = pow(scale * r, 1 / 2.2f);
    g = pow(scale * g, 1 / 2.2f);
    b = pow(scale * b, 1 / 2.2f);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * glm::clamp(r, 0.f, 0.999f)) << ' '
        << static_cast<int>(256 * glm::clamp(g, 0.f, 0.999f)) << ' '
        << static_cast<int>(256 * glm::clamp(b, 0.f, 0.999f)) << '\n';
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const unsigned int max_depth = 50;

    using std::make_shared;
    // World
    HitList world;
    
    auto material_ground = make_shared<Diffuse>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Diffuse>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_right = make_shared<Metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
    world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera
    Camera cam(vec3(0.f, 0.f, 0.f), 16.f / 9);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0.f, 0.f, 0.f);
            for (int s = 0; s < SAMPLES_PER_PIXEL; ++s) {
                float u = (i + random_float()) / (image_width - 1);
                float v = (j + random_float()) / (image_height - 1);
                ray r = cam.GetRay(u, v);
                pixel_color += static_cast<glm::vec3>(ray_color(r, world, max_depth));
            }
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}