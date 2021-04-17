#include <glad/glad.h>
#include "common.h"
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <cmath>
#include <fstream>
#include "GlfwApp.h"

std::string readGlsl( std::string name ) {
    std::ifstream in( "glsl\\" + name );
    std::string contents( ( std::istreambuf_iterator<char>( in ) ),
        std::istreambuf_iterator<char>() );
    return contents;
}

void loadShader( std::string& name, GLenum shaderType, GLuint program ) {
    std::map<GLenum, char*> knownTypes = { {GL_VERTEX_SHADER, ".vs"}, {GL_FRAGMENT_SHADER, ".fs"} };
    char* fileExt = knownTypes[shaderType];
    std::string main = readGlsl( name + fileExt );
    std::string random = readGlsl( "random.glsl" );
    std::string common = readGlsl( "common.glsl" );
    const GLchar* ptr[] = { common.c_str(), random.c_str(), main.c_str()  };
    auto shader = glCreateShader( shaderType );
    glShaderSource( shader, 3, ptr, NULL );

    glCompileShader( shader );

    GLint isCompiled = 0;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &isCompiled );
    if ( isCompiled == GL_FALSE ) {
        GLint maxLength = 0;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog( maxLength );
        glGetShaderInfoLog( shader, maxLength, &maxLength, &errorLog[0] );
        std::cout << name + fileExt << " compile failed\n";
        for ( auto i : errorLog )
            std::cout << i;

        glDeleteShader( shader );
    }

    glAttachShader( program, shader );
}

void loadGlProgram( std::string name ) {
    auto program = glCreateProgram();
    loadShader( name, GL_VERTEX_SHADER, program );
    loadShader( name, GL_FRAGMENT_SHADER, program );
    glLinkProgram( program );
    GLint link_ok = GL_FALSE;
    glGetProgramiv( program, GL_LINK_STATUS, &link_ok );
    if ( !link_ok ) {
        GLint maxLength = 0;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog( maxLength+1 );
        glGetProgramInfoLog( program, maxLength, &maxLength, &errorLog[0] );
        std::cout << name << " link failed\n";
        for ( auto i : errorLog )
            std::cout << i;
    }
    glUseProgram( program );
}

int main() {
    GlfwApp app;
    while ( app.run() ) {
        float viewAngle = atan( (float) app.SCR_HEIGHT / app.SCR_WIDTH ) * 2;

        bool u_limit = app.keyL;
        glUniform1i( 1, u_limit );

        glm::mat4 proj = glm::perspective( viewAngle, (float) app.SCR_WIDTH / app.SCR_HEIGHT, .1f, 10.f );
        auto projInv = glm::inverse( proj );
        auto screenPos = glm::vec4( app.xpos / app.SCR_WIDTH * 2 - 1, 1 - 2 * app.ypos / app.SCR_HEIGHT, 0, 0 );
        auto worldPos = projInv * screenPos;
        glUniform2fv( 2, 1, glm::value_ptr( worldPos ) );

        glUniformMatrix4fv( 3, 1, GL_FALSE, glm::value_ptr( proj ) );

        glm::mat4 view = glm::lookAt(
            glm::vec3( 0, 0, 1 ),
            glm::vec3( 0, 0, 0 ),
            glm::vec3( 0, 1, 0 )
        );
        glUniformMatrix4fv( 4, 1, GL_FALSE, glm::value_ptr( view ) );

        bool u_soften = app.keyS;
        glUniform1i( 5, u_soften );

        bool sendTime = app.keyT;
        if ( sendTime ) {
            glUniform1f( 6, (float) fmod( app.time, 1 ) );
        }

        float u_lightSize = app.scrollY;
        glUniform1f( 7, u_lightSize );
        draw();
    }
    return 0;
}
