#include <iostream>
#include "Ray.h"
#include <limits>
#include "Vec3.h"
#include "Hittable.h"
#include "Shapes.h"

#define INF std::numeric_limits<double>::infinity()

float hit_sphere(const vec3& center, double radius, const ray& r) {
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

vec3 ray_color(const ray& r, const Hittable& hitObj) {
    hit_record rec;
    if (hitObj.hit(r, 0, INF, rec)) {
        return 0.5f * (rec.normal + glm::vec3(1, 1, 1));
    }
    vec3 unit_direction = glm::normalize(r.direction);
    float t = 0.5f * (unit_direction.y + 1.0);
    return (1.f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

void write_color(std::ostream& out, glm::vec3 pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x) << ' '
        << static_cast<int>(255.999 * pixel_color.y) << ' '
        << static_cast<int>(255.999 * pixel_color.z) << '\n';
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // World
    HitList world;
    world.add(std::make_shared<Sphere>(vec3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(vec3(0, -100.5, -1), 100));

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    vec3 origin(0, 0, 0);
    vec3 horizontal(viewport_width, 0, 0);
    vec3 vertical(0, viewport_height, 0);
    vec3 lower_left_corner = origin - horizontal / 2.f - vertical / 2.f - vec3(0, 0, focal_length);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            float u = float(i) / (image_width - 1);
            float v = float(j) / (image_height - 1);
            ray r{ origin, lower_left_corner + u * horizontal + v * vertical - origin };
            vec3 pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}