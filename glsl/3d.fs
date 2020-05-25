#version 430

in vec4 var_position;

out vec4 FragColor;

layout( location = 0 ) uniform float intensity;
layout( location = 1 ) uniform bool limit;
layout( location = 2 ) uniform vec2 lightPos;
layout( location = 5 ) uniform bool soften;
layout( location = 6 ) uniform float time;

layout( std430, binding = 0 ) buffer layoutName
{
    int N;
    vec4 points[];
};

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

// Compound versions of the hashing algorithm I whipped together.
uint hash( uvec2 v ) { return hash( v.x ^ hash(v.y)                         ); }
uint hash( uvec3 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }
uint hash( uvec4 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }

// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}

// Pseudo-random value in half-open range [0:1].
float random( float x ) { return floatConstruct(hash(floatBitsToUint(x))); }
float random( vec2  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float random( vec3  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
float random( vec4  v ) { return floatConstruct(hash(floatBitsToUint(v))); }

const float PI = 3.1415926535897932384626433832795;

vec3 randomSeed1 = vec3(gl_FragCoord.xy, time);
vec3 randomSeed2 = vec3(gl_FragCoord.yx, 1-time);
float angle = random(randomSeed1) * 2*PI;
float radius = sqrt(random(randomSeed2) + 0.1);
vec2 softOffset = soften ? vec2(sin(angle),cos(angle)) : vec2(0);
vec3 lightPos3 = vec3(lightPos + softOffset * radius * 1e-2, 0.01);

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
