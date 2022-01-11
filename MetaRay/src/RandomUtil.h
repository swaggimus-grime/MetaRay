#ifndef RANDOM_UTIL_H
#define RANDOM_UTIL_H 

#include <random>
#include "CudaCore.h"
#include <curand_kernel.h>

#ifdef CUDA_ENABLED

class Random {
public:
    static void Init(curandState* rand_state, uint32_t numPixels);
    CUDA_SHARED static void PixelInit(curandState* rand_state, uint32_t pixelIndex);
};

#else
class Random {
public:
    static inline float Float();
    static inline float Float(float min, float max);
};

#endif

#endif