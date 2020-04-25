#include <glad/glad.h>
#include "common.h"

#ifdef CASE3D

const char* vertex_shader_text = MULTILINE(
#version 430\n

in vec4 in_position;

out vec4 var_position;

void main()
{
    gl_Position = in_position;
    var_position = in_position;
}
);

const char* fragment_shader_text = MULTILINE(
#version 430\n

in vec4 var_position;

layout( location = 0 ) uniform float intensity;
layout( location = 1 ) uniform bool limit;

layout( std430, binding = 3 ) buffer layoutName
{
    int N;
    vec2 points[];
};

bool ccw( vec2 A, vec2 B, vec2 C ) {
    return ( C.y - A.y ) * ( B.x - A.x ) > ( B.y - A.y )* ( C.x - A.x );
};

bool intersect( vec2 A, vec2 B, vec2 C, vec2 D ) {
    return ccw( A, C, D ) != ccw( B, C, D ) && ccw( A, B, C ) != ccw( A, B, D );
}

bool intersectsOne( vec2 point1, vec2 point2 ) {
    return intersect( vec2( 0 ), var_position.xy, point1, point2 );
}

bool intersectsAny() {
    int n = limit ? 2 : N;
    for ( int i = 0; i < n; i += 2 ) {
        if ( intersectsOne( points[i], points[i + 1] ) )
            return true;
    }
    return false;
}

void main()
{
    vec2 xy = vec2( N, N );
    gl_FragColor.rgb = vec3( 1 / ( 1 + length( var_position.xy ) ) );
    if ( intersectsAny() )
        gl_FragColor = vec4( 1, 0, 1, 1 );
    gl_FragColor *= intensity;
}
);

struct SSBO_DATA ssboData { VertexCount };

void init() {
    srand( 0xabc );
    auto& data = ssboData;
    for ( int i = 0; i < data.N; i += 2 ) {
        data.points[i][0] = floatRandom();
        data.points[i][1] = floatRandom();
        float n = data.points[i][0] * data.points[i][0] + data.points[i][1] * data.points[i][1];
        float nr = 1 / sqrt( sqrt( n ) );
        data.points[i + 1][0] = nr * data.points[i][0] + floatRandom() * .2f;
        data.points[i + 1][1] = nr * data.points[i][1] + floatRandom() * .2f;
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

#endif