in vec4 in_position;

out vec4 var_position;

void main()
{
    gl_Position = matProjection * matView * in_position;
    var_position = in_position;
}