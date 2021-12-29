#include <iostream>
#include "Ray.h"
#include <glm/glm.hpp>
#include "vec3util.h"

float hit_sphere(const glm::vec3& center, double radius, const ray& r) {
    glm::vec3 oc = r.origin - center;
    auto a = lensqr(r.direction);
    auto half_b = glm::dot(oc, r.direction);
    auto c = lensqr(oc) - radius * radius;
    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

glm::vec3 ray_color(const ray& r) {
    float t = hit_sphere(glm::vec3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        glm::vec3 N = glm::normalize(r.at(t) - glm::vec3(0, 0, -1));
        return 0.5f * glm::vec3(N.x + 1, N.y + 1, N.z + 1);
    }
    glm::vec3 unit_direction = glm::normalize(r.direction);
    t = 0.5 * (unit_direction.y + 1.0);
    return (1.f - t) * glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
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

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    glm::vec3 origin = glm::vec3(0, 0, 0);
    glm::vec3 horizontal = glm::vec3(viewport_width, 0, 0);
    glm::vec3 vertical = glm::vec3(0, viewport_height, 0);
    glm::vec3 lower_left_corner = origin - horizontal / 2.f - vertical / 2.f - glm::vec3(0, 0, focal_length);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            float u = float(i) / (image_width - 1);
            float v = float(j) / (image_height - 1);
            ray r{ origin, lower_left_corner + u * horizontal + v * vertical - origin };
            glm::vec3 pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}