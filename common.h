#pragma once

#include <vector>
#include <iostream>

#define floatRandom() (static_cast <float> ( rand() ) / static_cast <float> ( RAND_MAX ) * 2 - 1)

#define MULTILINE(...) #__VA_ARGS__

#if 1
#define CASE2D
#else
#define CASE3D
#endif

#if defined(CASE2D) && defined(CASE3D)
#error "Can only define one of CASE2D and CASE3D"
#endif

#ifdef CASE2D

const int VertexCount = 2 * 200;
struct SSBO_DATA { // SOB aligns to 8 :/
    int N;
    int crap;
    float points[VertexCount][2];
};

#endif

#ifdef CASE3D

const int VertexCount = 2 * 200;
struct SSBO_DATA { // SOB aligns to 8 :/
    int N;
    int crap;
    float points[VertexCount][2];
};

#endif

extern SSBO_DATA ssboData;

extern const char* vertex_shader_text;
extern const char* fragment_shader_text;

extern void init();
extern void draw();