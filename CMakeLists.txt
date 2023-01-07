# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.18)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(_USE_MATH_DEFINES)

project("MetaRay" LANGUAGES C CXX CUDA)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

message(STATUS "Working directory: ${PROJECT_SOURCE_DIR}")
message(STATUS "CUDA STANDARD: ${CMAKE_CUDA_STANDARD}")

add_subdirectory("MetaRay")

install(TARGETS MetaRay DESTINATION bin)