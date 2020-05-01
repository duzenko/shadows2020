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
