#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <cmath>
#include <fstream>
#include "GlfwApp.h"
#include "..\glhelpers\GlProgram.h"
#include "..\glhelpers\GlSsbo.h"
#include "../glhelpers/glhelpers.h"

const int VertexCount = 3 * 120;
glm::vec4 points[VertexCount];

void init() {
    srand( 0xabc );
    for ( int i = 0; i < VertexCount; i += 3 ) {
        auto& p1 = points[i], & p2 = points[i + 1], & p3 = points[i + 2];
        p1.x = frandom( -1 ) * .9f;
        p1.y = frandom( -1 ) * .9f;
        float a = frandom( 0, 2 * 3.14159f );
        float l = frandom( .03f, .1f );
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
            shader.time = (float) fmod( app.time, 1 );
        }

        shader.lightSize = app.scrollY;
        shader.vertexCount = VertexCount;

        draw();
    }
    return 0;
}
