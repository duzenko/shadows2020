#pragma once

#include <vector>
#include <iostream>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#define floatRandom() (static_cast <float> ( rand() ) / static_cast <float> ( RAND_MAX ) * 2 - 1)

#ifdef CASE
#error "ifdef name conflict"
#endif
#define CASE 3
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

#ifdef CASE2D

const int VertexCount = 2 * 400;
struct SSBO_DATA { 
    int N;
    int crap; // SOB aligns to 8 :/
    float points[VertexCount][2];
};

#endif

#ifdef CASE3D

const int VertexCount = 3 * 120;
struct SSBO_DATA {
    int N;
    int crap[3]; // aligns to vec4
    glm::vec4 points[VertexCount];
};

#endif

extern SSBO_DATA ssboData;

extern void init();
extern void compileShader( int shader );
extern void compileShaders();
extern void loadGlProgram( std::string name );
extern void draw();