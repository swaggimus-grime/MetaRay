# MetaRay

Example project for ray tracing in C++. Uses the guidance of Peter Shirley's Ray Tracing in One Weekend[(github)](https://github.com/RayTracing/raytracing.github.io/). 
For educational purposes only.

## Prerequisites and Building
This project uses CMake for building(version 3.18 required). This project also uses CUDA 12.0 for parallel processing. To build, use the command
```
cmake -B build
```
from the root directory.

## Run
To run the program, you can build the executable via the project generated with CMake.
The project generated will have command-line args so the run command will look like
```
MetaRay.exe > image.ppm
```
by default. The image created will be located in the same directory as the built project.
