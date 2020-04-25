#pragma once

#include <vector>
#include <iostream>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

#define floatRandom() (static_cast <float> ( rand() ) / static_cast <float> ( RAND_MAX ) * 2 - 1)

#if 0
#define CASE2D
#else
#define CASE3D
#endif

#if defined(CASE2D) && defined(CASE3D)
#error "Can only define one of CASE2D and CASE3D"
#endif

#ifdef CASE2D

const int VertexCount = 2 * 200;
struct SSBO_DATA { 
    int N;
    int crap; // SOB aligns to 8 :/
    float points[VertexCount][2];
};

#endif

#ifdef CASE3D

const int VertexCount = 3 * 40;
struct SSBO_DATA {
    int N;
    int crap[3]; // aligns to vec4
    glm::vec4 points[VertexCount];
};

#endif

extern SSBO_DATA ssboData;

extern const char* const vertex_shader_text;
extern const char* const fragment_shader_text;

extern void init();
extern void draw();