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

const float kEpsilon = 1e-3;

float rayTriangleIntersect( 
    vec3 orig, vec3 dir, 
    vec3 v0, vec3 v1, vec3 v2) 
{ 
    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;
    vec3 pvec = cross(dir, e2);
    float det = dot(e1, pvec);
    if (det < 1e-8 && det > -1e-8) {
        return 0;
    }
    float inv_det = 1 / det;
    vec3 tvec = orig - v0;
    float u = dot(tvec, pvec) * inv_det;
    if (u < 0 || u > 1) {
        return 0;
    }
    vec3 qvec = cross(tvec, e1);
    float v = dot(dir, qvec) * inv_det;
    if (v < 0 || u + v > 1) {
        return 0;
    }
    return dot(e2, qvec) * inv_det;
} 

bool intersectsOne( int i ) {
    //return distance(var_position.xyz, points[i].xyz) < .03|| distance(var_position.xyz, points[i+1].xyz) < .03|| distance(var_position.xyz, points[i+2].xyz) < .03;
    vec3 lightPos3 = vec3(lightPos, 0.01);
    float rayT = rayTriangleIntersect(lightPos3, var_position.xyz - lightPos3, points[i].xyz, points[i+1].xyz, points[i+2].xyz );
    if(rayT <= 0)
        return false;
    return rayT < 0.9999;
}

bool intersectsAny() {
    int n = limit ? 2 : N;
    for ( int i = 0; i < n; i += 3 ) {
        if ( intersectsOne(i) )
            return true;
    }
    return false;
}

void main()
{
    vec2 xy = vec2( N, N );
    FragColor.rgb = vec3( 1 / ( 1 + distance( var_position.xy, lightPos ) ) );
    if ( intersectsAny() )
        FragColor = vec4( .3, 0, .3, 1 );
    FragColor *= intensity;
    FragColor.g += var_position.z*3;
}
)"""";

struct SSBO_DATA ssboData { VertexCount };

void init() {
    srand( 0xabc );
    auto& data = ssboData;
    for ( int i = 0; i < data.N; i += 3 ) {
        auto& p1 = data.points[i], & p2 = data.points[i + 1], & p3 = data.points[i + 2];
        p1.x = floatRandom() * .9f;
        p1.y = floatRandom() * .9f;
        float a = floatRandom() * 2 * 3.14159;
        p2.x = p1.x + sin( a ) * .1f;
        p2.y = p1.y + cos( a ) * .1f;
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