cmake_minimum_required(VERSION 3.18)

# Add source to this project's executable.
file(GLOB cpu_source_files "src/*.cc")
file(GLOB gpu_source_files "src/*.cu")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src
					${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
)

find_package(CUDAToolkit)
add_executable(MetaRay ${cpu_source_files} ${gpu_source_files})
set_target_properties(MetaRay PROPERTIES VS_DEBUGGER_COMMAND_ARGUMENTS "> image.ppm")
set_target_properties(MetaRay
  PROPERTIES CUDA_SEPARABLE_COMPILATION ON 
)
target_link_libraries(MetaRay PRIVATE CUDA::cudart)
# target_precompile_headers(MetaRay PRIVATE src/pch.h)