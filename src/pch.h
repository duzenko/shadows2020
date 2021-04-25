#pragma once

#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "rnd.h"
#include "..\glhelpers\GlProgram.h"
#include "..\glhelpers\GlSsbo.h"
#include "..\glhelpers\GlPbo.h"
#include "../glhelpers/glhelpers.h"

const int PyramidCount = 20, TriangleCount = PyramidCount * 3, VertexCount = 3 * TriangleCount;

#include "GlfwWin.h"
#include "Shaders.h"
#include "App.h"