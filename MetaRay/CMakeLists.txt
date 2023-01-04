# CMakeList.txt : CMake project for MetaRay, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.22)

# Add source to this project's executable.
file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "src/*.h" "src/*.cpp")

add_compile_definitions(_USE_MATH_DEFINES)

set(VS_DEBUGGER_COMMAND_ARGUMENTS )

find_package(CUDAToolkit)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
					../tools/glm
					${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
)

add_executable(MetaRay ${SRC})
set_target_properties(MetaRay PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "> image.ppm")
set_target_properties(MetaRay
                       PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_link_libraries(MetaRay PRIVATE CUDA::cudart)
# target_precompile_headers(MetaRay PRIVATE src/pch.h)