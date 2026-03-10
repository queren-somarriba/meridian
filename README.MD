Outrenoir
Outrenoir is a generative art engine written in C++ and OpenGL. Inspired by the textured canvases and light play of the painter Pierre Soulages, this program generates real-time animations of organic strata, drawn using vector fields and natural placement algorithms.

The engine also features a direct video export pipeline (via pipe) to record lossless renders using FFmpeg.

🚀 Technical Features
This project was built from scratch without external game engines, implementing low-level mathematical and graphical algorithms:

Poisson Disk Sampling: Organic and harmonious generation of line starting points, preventing artificial clusters.

Flow Field (Perlin Noise & FBM): Calculation of fluid and textured trajectories simulating laminar or turbulent physics.

Spatial Hashing: Optimized collision detection system using a spatial grid to prevent geometry overlap with high performance.

Custom GLSL Shaders:

Dynamic lighting with randomized orientation on each execution.

Procedural generation of a "Canson Paper" texture using Fractal Brownian Motion (FBM) directly on the GPU.

Integrated Video Export: Inter-process communication (IPC) with FFmpeg to encode the OpenGL Framebuffer stream into a 60 FPS .mp4 file.

🛠️ Prerequisites and Installation
To compile and run this project, your system must have the following libraries installed:

C++ Compiler (supporting C++11 minimum)

OpenGL 3.3 Core Profile

GLFW 3 (Window and input management)

GLEW (OpenGL extension loading)

FFmpeg (Optional, required only for video export)

Compilation
A Makefile is provided at the root of the project. Simply use the standard command:

Bash
make
⚙️ Usage and Configuration
The program takes a configuration text file as an argument, allowing you to modify the mathematical and visual behavior of the engine without recompiling.

Bash
./outrenoir conf/config.txt
The Configuration File
The parser reads key: value pairs. Here are the available parameters:

Parameter	Description
window_width	Render window width (e.g., 1920).
window_height	Render window height (e.g., 1080).
perlin_scale	
Zoom level in the Perlin noise.


(< 0.01 for smooth curves, > 0.01 to reveal the mathematical grid).

step_size	Length (in pixels) of the segment computed at each iteration.
line_padding	Guaranteed empty space around each line (manages the collision zone).
pds_radius	Exclusion radius (in pixels) for the starting point of each line.
speed	Animation speed (number of new segments drawn per frame).
record	1 to record an outrenoir.mp4 file, 0 to disable recording.
💡 Rendering Tip (Pointillism): By setting an extremely low step_size (e.g., 0.0007) and a high speed, the engine will draw suspension points revealing the initial Poisson Disk Sampling placement.

🏗️ Code Architecture
The architecture is modular and divided by responsibilities:

outrenoir.cpp: Entry point, configuration parser, and main orchestrator.

engine.cpp: GLFW context initialization, external shader loading, OpenGL buffers, and render loop.

flowfield.cpp: Vector grid generation and geometry calculation (segments and triangles) with collision stopping.

poissonDiskSampling.cpp: Spatial distribution algorithm for origin points.

noise.cpp: Raw mathematical implementation of noise (Perlin) and gradients.

This should look great on your GitHub repository! Would you like me to help you draft the Makefile with the standard strict compilation flags (-Wall -Wextra -Werror) that are usually required for your school projects?