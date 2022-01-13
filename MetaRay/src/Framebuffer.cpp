#include "Framebuffer.h"

#include <iostream>

Framebuffer::Framebuffer(uint32_t width, uint32_t height)
    :m_Width(width), m_Height(height), m_Data(nullptr)
{
    checkCudaErrors(cudaMallocManaged((void**)&m_Data, m_Width * m_Height * sizeof(vec3)));
}

Framebuffer::Framebuffer(uint32_t width, uint32_t height, vec3* data)
    :m_Width(width), m_Height(height), m_Data(data) 
{
}

Framebuffer::~Framebuffer()
{
    checkCudaErrors(cudaFree(m_Data));
}

void Framebuffer::Write(vec3* data)
{
    m_Data = data;
}

void Framebuffer::Present() const
{
    if (!m_Data) return;

    std::cout << "P3\n" << m_Width << " " << m_Height << "\n255\n";
    for (int j = m_Height - 1; j >= 0; j--) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < m_Width; i++) {
            size_t pixel_index = j * m_Width + i;
            int ir = int(255.99 * m_Data[pixel_index].r);
            int ig = int(255.99 * m_Data[pixel_index].g);
            int ib = int(255.99 * m_Data[pixel_index].b);
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
