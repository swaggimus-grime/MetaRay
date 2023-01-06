# CMakeList.txt : CMake project for MetaRay, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.22)

set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

# Add source to this project's executable.
file(GLOB cpu_source_files "src/*.cc")
file(GLOB gpu_source_files "src/*.cu")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
					../tools/glm
					${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
)

add_executable(MetaRay ${cpu_source_files} ${gpu_source_files})
set_target_properties(MetaRay PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "> image.ppm")
set_target_properties(MetaRay
                       PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_link_libraries(MetaRay PRIVATE glm::glm)
# target_precompile_headers(MetaRay PRIVATE src/pch.h)