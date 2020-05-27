#pragma once

#include <vector>
#include <iostream>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#ifdef CASE
#error "ifdef name conflict"
#endif
#define CASE 2
#if CASE==2
#define CASE2D
#elif CASE==3
#define CASE3D
#else
#error "define out of range"
#endif
#undef CASE

#if defined(CASE2D) && defined(CASE3D)
#error "Can only define one of CASE2D and CASE3D"
#endif

extern void* ssboData;
extern int ssboSize;

extern void init();
extern void compileShaders();
extern void loadGlProgram( std::string name );
extern void draw();

extern float frandom( float start = 0, float end = 1 );
