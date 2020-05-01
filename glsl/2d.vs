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