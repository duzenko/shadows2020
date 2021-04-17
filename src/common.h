#pragma once

#include <vector>
#include <iostream>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>

extern void* ssboData;
extern int ssboSize;

extern void init();
extern void compileShaders();
extern void loadGlProgram( std::string name );
int main();
extern void draw();