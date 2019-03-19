# ThreeDee
Programação 3D

Requires freeglut.lib and glew32.lib (OpenGL 3.3).
Open the project in Visual Studio and compile either in Release x86 or in Debug x86.
You can find some configuration constants in ThreeDee.cpp.
* FILE is the path to the nff file to be rendered.
* EPSILON is the value to be used for displacing the origin of secondary rays, in order to avoid self-intersections.
* DEPTH_TRACE_LIMIT is the maximum recursion depth for the ray tracing algorithm.
