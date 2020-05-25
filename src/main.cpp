#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "common.h"
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <cmath>
#include <fstream>

// settings
int SCR_WIDTH = 1280;
int SCR_HEIGHT = 800;

bool u_limit = false;
bool u_soften = true;
bool sendTime = true;

void processInput( GLFWwindow* window )
{
    if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
        glfwSetWindowShouldClose( window, true );

    float viewAngle = true ? atan( (float)SCR_HEIGHT/SCR_WIDTH ) * 2 : 90;
    glm::mat4 proj = glm::perspective( viewAngle, (float)SCR_WIDTH / SCR_HEIGHT, .1f, 10.f );
    glUniformMatrix4fv( 3, 1, GL_FALSE, glm::value_ptr( proj ) );
    glm::mat4 view = glm::lookAt(
        glm::vec3( 0, 0, 1 ),
        glm::vec3( 0, 0, 0 ),
        glm::vec3( 0, 1, 0 )
    );
    glUniformMatrix4fv( 4, 1, GL_FALSE, glm::value_ptr( view ) );

    double xpos, ypos;
    glfwGetCursorPos( window, &xpos, &ypos );
    glfwGetFramebufferSize( window, &SCR_WIDTH, &SCR_HEIGHT );
    auto projInv = glm::inverse( proj );
    auto screenPos = glm::vec4( xpos/SCR_WIDTH * 2 - 1, 1 - 2 * ypos/SCR_HEIGHT, 0, 0 );
    auto worldPos = projInv * screenPos;
    glUniform2fv( 2, 1, glm::value_ptr( worldPos ) );

    if ( sendTime ) {
        double time = glfwGetTime();
        glUniform1f( 6, (float)fmod( time, 1 ) );
    }
    glUniform1i( 1, u_limit );
    glUniform1i( 5, u_soften );
}

void framebuffer_size_callback( GLFWwindow* window, int width, int height )
{
    glViewport( 0, 0, width, height );
}

void compileShader( int shader ) {
    glCompileShader( shader );

    GLint isCompiled = 0;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &isCompiled );
    if ( isCompiled == GL_FALSE ) {
        GLint maxLength = 0;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog( maxLength );
        glGetShaderInfoLog( shader, maxLength, &maxLength, &errorLog[0] );
        for ( auto i : errorLog )
            std::cout << i;

        glDeleteShader( shader );
    }
}

void loadShader( std::string& name, GLenum shaderType, GLuint program ) {
    std::map<GLenum, char*> knownTypes = { {GL_VERTEX_SHADER, ".vs"}, {GL_FRAGMENT_SHADER, ".fs"} };
    char* fileExt = knownTypes[shaderType];
    std::ifstream in( "glsl\\" + name + fileExt );
    std::string contents( ( std::istreambuf_iterator<char>( in ) ),
        std::istreambuf_iterator<char>() );
    auto ptr = contents.c_str();
    auto shader = glCreateShader( shaderType );
    glShaderSource( shader, 1, &ptr, NULL );
    compileShader( shader );
    glAttachShader( program, shader );
}

void loadGlProgram( std::string name ) {
    auto program = glCreateProgram();
    loadShader( name, GL_VERTEX_SHADER, program );
    loadShader( name, GL_FRAGMENT_SHADER, program );
    glLinkProgram( program );
    glUseProgram( program );
}

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    if ( action != GLFW_PRESS ) {
        return;
    }
    if ( key == GLFW_KEY_L ) {
        u_limit = !u_limit;
    }
    if ( key == GLFW_KEY_S ) {
        u_soften = !u_soften;
    }
    if ( key == GLFW_KEY_T ) {
        sendTime = !sendTime;
    }
}

int main()
{
    glfwInit();
    glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );
    GLFWwindow* window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL );
    if ( window == NULL ) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent( window );
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );
    glfwSetKeyCallback( window, key_callback );

    if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) ) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSwapInterval( 1 );

    init();

    GLuint ssbo;
    glGenBuffers( 1, &ssbo );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, ssbo );
    glBufferData( GL_SHADER_STORAGE_BUFFER, sizeof( ssboData ), &ssboData, GL_DYNAMIC_COPY );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, ssbo );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );

    compileShaders();

    while ( !glfwWindowShouldClose( window ) ) {
        processInput( window );
        draw();
        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

extern "C" {
    __declspec(dllexport) int NvOptimusEnablement = 0x00000001;
    __declspec( dllexport ) int AmdPowerXpressRequestHighPerformance = 1;
}