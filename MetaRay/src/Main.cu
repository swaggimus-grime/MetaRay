#include <iostream>
#include "ray.h"
#include "Shapes.h"
#include "Hittable.h"
#include "Framebuffer.h"
#include <curand_kernel.h>
#include "TimeInstrumentation.h"

__device__ vec3 random_in_unit_sphere(curandState* randState) {
    while (true) {
        vec3 p = 2.f * vec3::random(randState) - 1.f;
        if (p.lensqr() >= 1) continue;
        return p;
    }
}

__device__ vec3 ray_color(const ray& r, Hittable** world, curandState* local_rand_state) {
    ray cur_ray = r;
    float cur_attenuation = 1.0f;
    for (int i = 0; i < 50; i++) {
        hit_record rec;
        if ((*world)->hit(cur_ray, 0.001f, FLT_MAX, rec)) {
            vec3 target = rec.p + rec.normal + random_in_unit_sphere(local_rand_state);
            cur_attenuation *= 0.5f;
            cur_ray = ray(rec.p, target - rec.p);
        }
        else {
            vec3 unit_direction = cur_ray.direction.unit();
            float t = 0.5f * (unit_direction.y + 1.0f);
            vec3 c = (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
            return cur_attenuation * c;
        }
    }
    return vec3(0.0, 0.0, 0.0); // exceeded recursion
}

__global__ void render(vec3* fb, uint32_t max_x, uint32_t max_y,
    vec3 lower_left_corner, vec3 horizontal, vec3 vertical, vec3 origin,
    Hittable** world, curandState* rand_state) {
    int i = threadIdx.x + blockIdx.x * blockDim.x;
    int j = threadIdx.y + blockIdx.y * blockDim.y;
    if ((i >= max_x) || (j >= max_y)) return;
    int pixel_index = j * max_x + i;
    curand_init(1984, pixel_index, 0, &rand_state[pixel_index]);
    float u = float(i + curand_uniform(&rand_state[pixel_index])) / float(max_x);
    float v = float(j + curand_uniform(&rand_state[pixel_index])) / float(max_y);
    ray r(origin, lower_left_corner + u * horizontal + v * vertical);
    fb[pixel_index] = ray_color(r, world, &rand_state[pixel_index]);
    fb[pixel_index].r = pow(fb[pixel_index].r, 1.f / 2.2f);
    fb[pixel_index].b = pow(fb[pixel_index].b, 1.f / 2.2f);
    fb[pixel_index].g = pow(fb[pixel_index].g, 1.f / 2.2f);
}

__global__ void create_world(Hittable** d_list, Hittable** d_world) {
    if (threadIdx.x == 0 && blockIdx.x == 0) {
        *(d_list) = new Sphere(vec3(0, 0, -1), 0.5);
        *(d_list + 1) = new Sphere(vec3(0, -100.5, -1), 100);
        *d_world = new HitList(d_list, 2);
    }
}

__global__ void free_world(Hittable** d_list, Hittable** d_world) {
    delete* (d_list);
    delete* (d_list + 1);
    delete* d_world;
}

int main() {
    const uint32_t nx = 1200;
    const uint32_t ny = 600;
    const uint16_t tx = 8;
    const uint16_t ty = 8;

    std::cerr << "Rendering a " << nx << "x" << ny << " image ";
    std::cerr << "in " << tx << "x" << ty << " blocks.\n";

    uint32_t num_pixels = nx * ny;
    size_t fb_size = num_pixels * sizeof(vec3);

    curandState* s_rand_state;
    checkCudaErrors(cudaMalloc((void**)&s_rand_state, num_pixels * sizeof(curandState)));

    // allocate FB
    Framebuffer fbo(nx, ny);

    // make our world of hitables
    Hittable** d_list;
    checkCudaErrors(cudaMalloc((void**)&d_list, 2 * sizeof(Hittable*)));
    Hittable** d_world;
    checkCudaErrors(cudaMalloc((void**)&d_world, sizeof(Hittable*)));
    create_world<<<1, 1>>>(d_list, d_world);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    TimeInst::Start();
    // Render our buffer
    dim3 blocks(nx / tx + 1, ny / ty + 1);
    dim3 threads(tx, ty);
    render << <blocks, threads >> > (fbo.m_Data, nx, ny,
        vec3(-2.0, -1.0, -1.0),
        vec3(4.0, 0.0, 0.0),
        vec3(0.0, 2.0, 0.0),
        vec3(0.0, 0.0, 0.0),
        d_world, s_rand_state);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());
    std::cerr << "took " << TimeInst::End() << " seconds.\n";

    // Output FB as Image
    fbo.Present();

    // clean up
    checkCudaErrors(cudaDeviceSynchronize());
    free_world << <1, 1 >> > (d_list, d_world);
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaFree(d_list));
    checkCudaErrors(cudaFree(d_world));

    // useful for cuda-memcheck --leak-check full
    cudaDeviceReset();
    return 0;
}
