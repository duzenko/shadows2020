in vec4 var_position;

out vec4 FragColor;

layout( std430, binding = 0 ) buffer layoutName
{
    vec4 points[];
};

vec2 softOffset = vec2(0);
vec3 lightPos3 = vec3(lightPos, 0.01);

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
    float rayT = rayTriangleIntersect(lightPos3, var_position.xyz - lightPos3, points[i].xyz, points[i+1].xyz, points[i+2].xyz );
    if(rayT <= 0)
        return false;
    return rayT < 0.9999;
}

bool intersectsAny() {
    int n = limit ? 2 : vertexCount;
    for ( int i = 0; i < n; i += 3 ) {
        if ( intersectsOne(i) )
            return true;
    }
    return false;
}

float getLit() {
    if(soften==0)
        return intersectsAny() ? 0 : 1;
    float lit = 0;
    for(int i=0; i<soften; i++) {
        vec3 randomSeed1 = vec3(gl_FragCoord.xy+i+time, i+time);
        vec3 randomSeed2 = vec3(gl_FragCoord.yx-i-time, -i-time);
        float angle = random(randomSeed1) * 2*PI;
        float radius = sqrt(random(randomSeed2) + 0.1);
        softOffset = vec2(sin(angle),cos(angle));
        lightPos3 = vec3(lightPos + softOffset * radius * lightSize, lightSize);
        lit += intersectsAny() ? 0 : 1;
    }
    return lit / soften;
}

void main()
{
    float lit = getLit();
    FragColor.rgb = vec3(lit);
}