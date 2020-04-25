#pragma once

#include <vector>
#include <iostream>

#define floatRandom() (static_cast <float> ( rand() ) / static_cast <float> ( RAND_MAX ) * 2 - 1)

#define MULTILINE(...) #__VA_ARGS__

#if 1

const int VertexCount = 2 * 200;
struct SSBO2D { // SOB aligns to 8 :/
    int N;
    int crap;
    float points[VertexCount][2];
};
extern SSBO2D ssboData2d;

extern const char* vertex_shader_text;
extern const char* fragment_shader_text;

extern void init2d();
extern void draw2d();

#define init init2d
#define draw draw2d
#define ssboData ssboData2d

#endif