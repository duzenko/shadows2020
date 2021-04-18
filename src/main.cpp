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
#include "..\glhelpers\GlProgram.h"
#include "..\glhelpers\GlSsbo.h"
#include "../glhelpers/glhelpers.h"

const int PyramidCount = 20, TriangleCount = PyramidCount * 3, VertexCount = 3 * TriangleCount;
glm::vec4 points[VertexCount];

extern float frandom( float start = 0, float end = 1 );

void randomPyramid( glm::vec4 vertices[] ) {
    glm::vec4 apex = {}, b[3] = {};
    apex.x = frandom( -1 ) * .6f;
    apex.y = frandom( -1 ) * .6f;
    apex.z = 5e-3;
    for ( int i = 0; i < 3; i++ ) {
        glm::vec4 &p2 = b[i];
        float a = frandom( 0, 2 * (float) M_PI );
        a = 2 * (float) M_PI * i / 3;
        float l = frandom( .03f, .1f );
        p2.x = apex.x + sin( a ) * l;
        p2.y = apex.y + cos( a ) * l;
    }
    vertices[0] = vertices[3] = vertices[6] = apex;
    vertices[1] = vertices[8] = b[0];
    vertices[2] = vertices[4] = b[1];
    vertices[5] = vertices[7] = b[2];
}

void init() {
    srand( 0xabcdef );
    glm::vec4 pyramid[9];
    for ( int pyra = 0; pyra < PyramidCount; pyra ++ ) {
        randomPyramid( pyramid );
        std::copy( pyramid, pyramid + 9, points + pyra * 9 );
    }
}

void setup( GlProgram &shader, GlfwApp &app ) {
    shader.limit = app.keyL;

    float viewAngle = atan( (float) app.SCR_HEIGHT / app.SCR_WIDTH ) * 2;
    glm::mat4 proj = glm::perspective( viewAngle, (float) app.SCR_WIDTH / app.SCR_HEIGHT, .1f, 10.f );
    auto projInv = glm::inverse( proj );
    auto screenPos = glm::vec4( app.xpos / app.SCR_WIDTH * 2 - 1, 1 - 2 * app.ypos / app.SCR_HEIGHT, 0, 0 );
    auto worldPos = projInv * screenPos;
    shader.lightPos = glm::vec2( worldPos );

    shader.matProjection = proj;

    glm::mat4 view = glm::lookAt(
        glm::vec3( 0, 0, 1 ),
        glm::vec3( 0, 0, 0 ),
        glm::vec3( 0, 1, 0 )
    );
    shader.matView = view;

    shader.soften = app.keyS;

    bool sendTime = app.keyT;
    if ( sendTime ) {
        shader.time = (float) fmod( app.time, 2*M_PI );
    }

    shader.lightSize = app.scrollY;
    shader.vertexCount = VertexCount;
}

void draw() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUniform1f( 0, 0.3f );
    glBegin( GL_QUADS );
    glVertex2i( -1, -1 );
    glVertex2i( 1, -1 );
    glVertex2i( 1, 1 );
    glVertex2i( -1, 1 );
    glEnd();

    glUniform1f( 0, 1 );
    glLineWidth( 2 );
    glBegin( GL_TRIANGLES );
    for ( int i = 0; i < VertexCount; i++ ) {
        glVertex3fv( glm::value_ptr( points[i] ) );
    }
    glEnd();
}

int main() {
    init();
    GlfwApp app;
    GlProgram shader;
    GlSsbo ssbo( sizeof( points ), points );
    while ( app.run() ) {
        setup( shader, app );
        draw();
    }
    return 0;
}
