#version 430

layout( location = 0 ) uniform float intensity;
layout( location = 1 ) uniform bool limit;
layout( location = 2 ) uniform vec2 lightPos;
layout( location = 3 ) uniform mat4 matProjection;
layout( location = 4 ) uniform mat4 matView;
layout( location = 5 ) uniform bool soften;
layout( location = 6 ) uniform float time;
layout( location = 7 ) uniform float lightSize;

const float PI = 3.1415926535897932384626433832795;
