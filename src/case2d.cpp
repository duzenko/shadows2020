#include <glad/glad.h>
#include "common.h"

#ifdef CASE2D

const int VertexCount = 2 * 400;
struct SSBO_DATA {
    int N;
    int crap; // SOB aligns to 8 :/
    float points[VertexCount][2];
};

struct SSBO_DATA data { VertexCount };

void* ssboData = &data;
int ssboSize = sizeof( data );

void init() {
    srand( 0xabc );
    for ( int i = 0; i < data.N; i += 2 ) {
        data.points[i][0] = floatRandom();
        data.points[i][1] = floatRandom();
        data.points[i + 1][0] = data.points[i][0] + floatRandom() * .1f;
        data.points[i + 1][1] = data.points[i][1] + floatRandom() * .1f;
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
    for ( int i = 0; i < data.N; i++ ) {
        glVertex2fv( &data.points[i][0] );
    }
    glEnd();
}

void compileShaders() {
    loadGlProgram( "2d" );
}

#endif