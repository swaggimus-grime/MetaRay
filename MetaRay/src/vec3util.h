#pragma once

#include <glm/glm.hpp>

double lensqr(const glm::vec3& v) { return pow(glm::length(v), 2); }