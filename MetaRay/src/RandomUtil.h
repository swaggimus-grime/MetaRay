#ifndef RANDOM_UTIL_H
#define RANDOM_UTIL_H 

#include <random>
#include "CudaCore.h"

#ifdef CUDA_ENABLED

#else
class Random {
public:
    static inline float Float();
    static inline float Float(float min, float max);
};

#endif

#endif