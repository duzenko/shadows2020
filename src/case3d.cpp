#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "common.h"

#ifdef CASE3D

const int VertexCount = 3 * 120;
struct SSBO_DATA {
    int N;
    int crap[3]; // aligns to vec4
    glm::vec4 points[VertexCount];
};

struct SSBO_DATA data { VertexCount };

void* ssboData = &data;
int ssboSize = sizeof( data );

void init() {
    srand( 0xabc );
    for ( int i = 0; i < data.N; i += 3 ) {
        auto& p1 = data.points[i], & p2 = data.points[i + 1], & p3 = data.points[i + 2];
        p1.x = frandom(-1) * .9f;
        p1.y = frandom(-1) * .9f;
        float a = frandom(0, 2 * 3.14159f);
        float l = frandom(.03f, .1f);
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
    for ( int i = 0; i < data.N; i++ ) {
        glVertex3fv( glm::value_ptr( data.points[i] ) );
    }
    glEnd();
}

void compileShaders() {
    loadGlProgram( "3d" );
}

#endif