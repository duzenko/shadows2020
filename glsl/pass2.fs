in vec4 var_position;

out vec4 FragColor;

void main()
{
    FragColor.rgb = vec3( 1 / ( 1 + distance( var_position.xy, lightPos ) ) );
    FragColor *= intensity;
    FragColor.g += .2-.2*cos(var_position.z*6e2);
    if(distance(distance(var_position.xy, lightPos)/lightSize, 1) < 0.1)
        FragColor.rgb = vec3(1, 0, 0);
}