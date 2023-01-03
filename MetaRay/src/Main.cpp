#include <iostream>
#include "Ray.h"
#include <limits>
#include "Vec3.h"
#include "Hittable.h"
#include "Shapes.h"
#include "Camera.h"
#include "Material.h"
#include <math.h>

using std::make_shared;
using std::shared_ptr;

#define SAMPLES_PER_PIXEL 4
#define INF std::numeric_limits<float>::infinity()

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
        return color(0.f, 0.f, 0.f);

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
    return (1.f - t) * color(1.f, 1.f, 1.f) + t * color(0.5f, 0.7f, 1.f);
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

HitList random_scene() {
    HitList world;

    auto ground_material = make_shared<Diffuse>(color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = Random::Float();
            point3 center(a + 0.9 * Random::Float(), 0.2, b + 0.9 * Random::Float());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<Diffuse>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = Random::Float(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Diffuse>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

void generate_image(uint32_t width, uint32_t height, Camera& camera, Hittable& hitObj, const uint32_t maxRecursionDepth)
{
    std::cout << "P3\n" << width << " " << height << "\n255\n";
    for (int j = height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < width; ++i) {
            color pixel_color{};
            for (int s = 0; s < SAMPLES_PER_PIXEL; ++s) {
                float u = (i + Random::Float()) / (width - 1);
                float v = (j + Random::Float()) / (height - 1);
                ray r = camera.LookAt(u, v);
                pixel_color = pixel_color + ray_color(r, hitObj, maxRecursionDepth);
            }
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}

int main() {

    // Image

    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int max_depth = 50;

    // World

    auto world = random_scene();

    // Camera

    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    generate_image(image_width, image_height, cam, world, max_depth);
}