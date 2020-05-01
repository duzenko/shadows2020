#version 430

in vec4 var_position;

layout( location = 0 ) uniform float intensity;
layout( location = 1 ) uniform bool limit;
layout( location = 2 ) uniform vec2 lightPos;

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
    return intersect( lightPos, var_position.xy, point1, point2 );
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
    gl_FragColor.rgb = vec3( 1 / ( 1 + distance( var_position.xy, lightPos ) ) );
    if ( intersectsAny() )
        gl_FragColor = vec4( 1, 0, 1, 1 );
    gl_FragColor *= intensity;
}