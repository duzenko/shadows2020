#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "common.h"

#ifdef CASE3D

struct SSBO_DATA ssboData { VertexCount };

void init() {
    srand( 0xabc );
    auto& data = ssboData;
    for ( int i = 0; i < data.N; i += 3 ) {
        auto& p1 = data.points[i], & p2 = data.points[i + 1], & p3 = data.points[i + 2];
        p1.x = floatRandom() * .9f;
        p1.y = floatRandom() * .9f;
        float a = floatRandom() * 2 * 3.14159f;
        float l = floatRandom() * .1 + .05;
        p2.x = p1.x + sin( a ) * l;
        p2.y = p1.y + cos( a ) * l;
        p3 = ( p1 + p2 ) * .5f;
        p3.z = .1f;
    }
}

void draw() {
    glClear( GL_COLOR_BUFFER_BIT );

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
    for ( int i = 0; i < ssboData.N; i++ ) {
        glVertex3fv( glm::value_ptr( ssboData.points[i] ) );
    }
    glEnd();
}

void compileShaders() {
    loadGlProgram( "3d" );
}

#endif