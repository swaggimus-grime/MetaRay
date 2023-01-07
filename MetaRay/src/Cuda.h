#ifndef CUDA_H
#define CUDA_H

#include <cuda_runtime.h>
#include <iostream>

#if defined(_WIN32)
#define DEBUG_BREAK() __debugbreak()
#endif

#define CUDA_SHARED __host__ __device__

#define checkCudaErrors(val) check_cuda( (val), #val, __FILE__, __LINE__ )
void check_cuda(cudaError_t result, char const* const func, const char* const file, int const line);

#define CUDA_ENABLED

#endif