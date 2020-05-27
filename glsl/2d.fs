in vec4 var_position;

layout( std430, binding = 0 ) buffer layoutName
{
    int N;
    vec2 points[];
};

vec3 randomSeed1 = vec3(gl_FragCoord.xy, time);
vec3 randomSeed2 = vec3(gl_FragCoord.yx, 1-time);
float angle = random(randomSeed1) * 2*PI;
float radius = sqrt(random(randomSeed2) + 0.1);
vec2 softOffset = soften ? vec2(sin(angle),cos(angle)) : vec2(0);
vec2 lightSamplePos = lightPos + softOffset * radius * lightSize;

bool ccw( vec2 A, vec2 B, vec2 C ) {
    return ( C.y - A.y ) * ( B.x - A.x ) > ( B.y - A.y ) * ( C.x - A.x );
};

bool intersect( vec2 A, vec2 B, vec2 C, vec2 D ) {
    return ccw( A, C, D ) != ccw( B, C, D ) && ccw( A, B, C ) != ccw( A, B, D );
}

bool intersectsOne( vec2 point1, vec2 point2 ) {
    //return distance(point1, point2 ) < .1;
    return intersect( lightSamplePos, var_position.xy, point1, point2 );
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