#include "RandomUtil.h"

#ifdef CUDA_ENABLED

void Random::Init(curandState* rand_state, uint32_t numPixels)
{
	checkCudaErrors(cudaMalloc((void**)&rand_state, numPixels * sizeof(curandState)));
}

CUDA_SHARED void Random::PixelInit(curandState* rand_state, uint32_t pixelIndex)
{
	curand_init(1984, pixelIndex, 0, &rand_state[pixelIndex]);
}

#else

static std::uniform_real_distribution<float> distribution;
static std::mt19937 generator;

float Random::Float()
{
	distribution = std::uniform_real_distribution<float>();
	return distribution(generator);
}

float Random::Float(float min, float max)
{
	distribution = std::uniform_real_distribution<float>(min, max);
	return distribution(generator);
}


#endif
