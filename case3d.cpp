#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "common.h"

#ifdef CASE3D

const char* const vertex_shader_text = R""""(
#version 430

in vec4 in_position;

out vec4 var_position;

layout( location = 3 ) uniform mat4 matProjection;
layout( location = 4 ) uniform mat4 matView;

void main()
{
    gl_Position = matProjection * matView * in_position;
    var_position = in_position;
}
)"""";

const char* const fragment_shader_text = R""""(
#version 430

in vec4 var_position;

out vec4 FragColor;

layout( location = 0 ) uniform float intensity;
layout( location = 1 ) uniform bool limit;
layout( location = 2 ) uniform vec2 lightPos;

layout( std430, binding = 3 ) buffer layoutName
{
    int N;
    vec4 points[];
};

bool ccw( vec2 A, vec2 B, vec2 C ) {
    return ( C.y - A.y ) * ( B.x - A.x ) > ( B.y - A.y )* ( C.x - A.x );
};

bool intersect( vec2 A, vec2 B, vec2 C, vec2 D ) {
    return ccw( A, C, D ) != ccw( B, C, D ) && ccw( A, B, C ) != ccw( A, B, D );
}

bool intersectsOne( vec2 point1, vec2 point2 ) {
    //return distance(var_position.xy, point1) < .01 || distance(var_position.xy, point2) < .01;
    return intersect( lightPos, var_position.xy, point1, point2 );
}

bool intersectsAny() {
    int n = limit ? 2 : N;
    for ( int i = 0; i < n; i += 3 ) {
        if ( intersectsOne( points[i].xy, points[i + 1].xy ) )
            return true;
    }
    return false;
}

void main()
{
    vec2 xy = vec2( N, N );
    FragColor.rgb = vec3( 1 / ( 1 + distance( var_position.xy, lightPos ) ) );
    FragColor.b *= 1 - var_position.z*10;
    if ( intersectsAny() )
        FragColor = vec4( 1, 0, 1, 1 );
    FragColor *= intensity;
}
)"""";

struct SSBO_DATA ssboData { VertexCount };

void init() {
    srand( 0xabc );
    auto& data = ssboData;
    for ( int i = 0; i < data.N; i += 3 ) {
        auto& p1 = data.points[i], & p2 = data.points[i + 1], & p3 = data.points[i + 2];
        p1.x = floatRandom();
        p1.y = floatRandom();
        p2.x = p1.x + floatRandom() * .2f;
        p2.y = p1.y + floatRandom() * .2f;
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

#endif