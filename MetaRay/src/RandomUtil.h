#ifndef RANDOM_UTIL_H
#define RANDOM_UTIL_H 

#include "Cuda.h"
#include <curand_kernel.h>

class Random {
public:
    __device__ static inline float Float(curandState* local_rand_state) {
        return curand_uniform(local_rand_state);
    }

    __device__ static inline float Float(curandState* local_rand_state, float min, float max) {
        float myrandf = curand_uniform(local_rand_state);
        myrandf *= (max - min + 0.999999);
        myrandf += min;
        return myrandf;
    }
};

#endif