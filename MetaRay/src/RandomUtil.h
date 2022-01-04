#pragma once

#include <random>

class Random {
public:
    static inline float Float() {
        static std::uniform_real_distribution<float> distribution(0.f, 1.f);
        static std::mt19937 generator;
        return distribution(generator);
    }

    static inline float Float(float min, float max) {
        static std::uniform_real_distribution<float> distribution(min, max);
        static std::mt19937 generator;
        return distribution(generator);
    }
};