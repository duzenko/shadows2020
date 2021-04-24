#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <fstream>
#include "GlfwApp.h"
#include "rnd.h"
#include "..\glhelpers\GlProgram.h"
#include "..\glhelpers\GlSsbo.h"
#include "../glhelpers/glhelpers.h"

const int PyramidCount = 20, TriangleCount = PyramidCount * 3, VertexCount = 3 * TriangleCount;
glm::vec4 points[VertexCount];

// create the pyramid vertices
void initGeometry() {
    srand( 0xabcdef );
    glm::vec4 pyramid[9];
    for ( int pyra = 0; pyra < PyramidCount; pyra ++ ) {
        randomPyramid( pyramid );
        std::copy( pyramid, pyramid + 9, points + pyra * 9 );
    }
}

// shader uniforms
void setup( GlProgram &shader, GlfwApp &app ) {
    // speed test - one triangle only
    shader.limit = app.keyL;

    // GLSL routine - projection matrix
    float fovY = atan( (float) app.SCR_HEIGHT / app.SCR_WIDTH ) * 2;
    glm::mat4 proj = glm::perspective( fovY, (float) app.SCR_WIDTH / app.SCR_HEIGHT, .1f, 10.f );
    shader.matProjection = proj;

    // GLSL routine - view matrix
    glm::mat4 view = glm::lookAt(
        glm::vec3( 0, 0, 1 ),
        glm::vec3( 0, 0, 0 ),
        glm::vec3( 0, 1, 0 )
    );
    shader.matView = view;

    // light follows mouse cursor
    auto projInv = glm::inverse( proj );
    auto screenPos = glm::vec4( app.xpos / app.SCR_WIDTH * 2 - 1, 1 - 2 * app.ypos / app.SCR_HEIGHT, 0, 0 );
    auto worldPos = projInv * screenPos;
    shader.lightPos = glm::vec2( worldPos );

    // softener level
    shader.soften = app.keyS;

    // stop time noise
    bool sendTime = app.keyT;
    if ( sendTime ) {
        shader.time = (float) fmod( app.time, 2*M_PI );
    }

    // light size
    shader.lightSize = app.scrollY;

    // SSBO size info
    shader.vertexCount = VertexCount;
}

void draw() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // canvas
    glUniform1f( 0, 0.3f );
    glBegin( GL_QUADS );
    glVertex2i( -1, -1 );
    glVertex2i( 1, -1 );
    glVertex2i( 1, 1 );
    glVertex2i( -1, 1 );
    glEnd();

    // pyramids
    glUniform1f( 0, 1 );
    glBegin( GL_TRIANGLES );
    for ( int i = 0; i < VertexCount; i++ ) {
        glVertex3fv( glm::value_ptr( points[i] ) );
    }
    glEnd();
}

int main() {
    initGeometry();
    GlfwApp app;
    GlProgram shader;
    GlSsbo ssbo( sizeof( points ), points );
    while ( app.run() ) {
        setup( shader, app );
        draw();
    }
    return 0;
}
