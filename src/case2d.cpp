#include <glad/glad.h>
#include "common.h"

#ifdef CASE2D

struct SSBO_DATA ssboData { VertexCount };

void init() {
    srand( 0xabc );
    auto &data = ssboData;
    for ( int i = 0; i < data.N; i += 2 ) {
        data.points[i][0] = floatRandom();
        data.points[i][1] = floatRandom();
        data.points[i + 1][0] = data.points[i][0] + floatRandom() * .2f;
        data.points[i + 1][1] = data.points[i][1] + floatRandom() * .2f;
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
    glBegin( GL_LINES );
    for ( int i = 0; i < ssboData.N; i++ ) {
        glVertex2fv( &ssboData.points[i][0] );
    }
    glEnd();
}

void compileShaders() {
    loadGlProgram( "2d" );
}

#endif