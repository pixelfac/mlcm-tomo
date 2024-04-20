# Introduction

This project was created by Nathan Harris as a member of team Mean Lean Coding Machine, in Spring 2024 as a part of Nathan's senior project under Professor Reza Entezari at the University of Florida

# File Directory

### main.cpp
Where all of the setup and configuration of OGL happens. It is also the only CPU access point in the algorithm, everything else is in the GPU

### importShaders.cpp
helper script to simplify the process of importing shader source files

### fanbeamfs.cpp
This file is the CPU scripting equivalent of fanbeam.fs, which I created as a means of debugging fanbeam.fs

### OpenGL libs
GL, GLFW, and GLM are all downloaded libraries for OpenGL and various extensions used in the project

## Shaders

### fanbeam.vs
Takes in vertex coords and moves/rotates them to the right position to form the correct triangle for the current view/detector pixel, passes vertices to fanbeam.gs, the geometry shader

### fanbeam.gs
Takes in vertices from vertex shader, 3 at a time. Groups them together into an array of 3 coords and passes that to fanbeam.fs, the frag shader, so it has all the data it needs to do area calcs. Usually the frag shader interpolates data it gets from the vertex shader, so I used this intermediate step to guarantee no interpolation of data and that the frag shader had all 3 orginal vertices for each primitive. Frag shader knows which primitive each pixel corresponds to, so the array of vertices, `positions`, is updated in the frag shader to have the right vertices for the primitive overlapping the current pixel.

### fanbeam.fs
Using the current pixel position and the geometry of the fanbeam (given as a triangle by the 3 vertices in `positions`), calculates the area that the fanbeam overlaps that pixel. The method of calculation is similar to integration, or summing areas of squares and triangles. It finds the area underneath the top border of the pixel/upper line. Basically, areaUnder(min(upperPixelBorder, upperLineOfFanbeam)) from the left to the right of the pixel (which itself is adding or subtracting triangles or trapezoids of squares formed by the intersecting lines of the upperPixelBorder and upperLineOfFanbeam). THEN, it finds that same area for the lower side of the pixel, and finds the difference between them. This logic also attempts to account for vertical/horizontal lines.

HOWEVER, this math is sorely untested and should be revised/verified for correct output (such as by using fanbeamfs.cpp)

### dotproduct.vs
Almost identical to fanbeam.vs, with the key difference that it passes a texture coordinate UV value to the frag shader so the frag shader knows where to pull pixels from in the rendered_texture

### dotproduct.fs
Using the UV value from the vertex shader, colors the screen with that color from the rendered_texture variable. rendered_texture is, of course, the rendered output from fanbeam.fs, written to a texture

### passthrough.vs
Passes a vertex position and UV value through to frag shader

### texture.vs
draws pixels from texture variable to screen

### gradient.fs
Colors screen using interpolated vertex positions from vertex shader
