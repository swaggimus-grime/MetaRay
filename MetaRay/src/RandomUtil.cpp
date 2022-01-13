#include "RandomUtil.h"

#ifdef CUDA_ENABLED

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
