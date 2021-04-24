#pragma once

float frandom( float start = 0, float end = 1 ) {
    float range = end - start;
    return start + range * static_cast <float> ( rand() ) / static_cast <float> ( RAND_MAX );
}

void randomPyramid( glm::vec4 vertices[] ) {
    glm::vec4 apex = {}, b[3] = {};
    apex.x = frandom( -1 ) * .6f;
    apex.y = frandom( -1 ) * .6f;
    apex.z = 5e-2f;
    for ( int i = 0; i < 3; i++ ) {
        glm::vec4& p2 = b[i];
        float a = frandom( 0, 2 * (float) M_PI );
        a = 2 * (float) M_PI * i / 3;
        float l = frandom( .03f, .1f );
        p2.x = apex.x + sin( a ) * l;
        p2.y = apex.y + cos( a ) * l;
    }
    vertices[0] = vertices[3] = vertices[6] = apex;
    vertices[1] = vertices[8] = b[0];
    vertices[2] = vertices[4] = b[1];
    vertices[5] = vertices[7] = b[2];
}