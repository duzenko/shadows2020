#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "common.h"
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

// settings
int SCR_WIDTH = 1280;
int SCR_HEIGHT = 800;

void processInput( GLFWwindow* window )
{
    if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
        glfwSetWindowShouldClose( window, true );
    if ( glfwGetKey( window, GLFW_KEY_1 ) == GLFW_PRESS )
        glUniform1i( 1, 0 );
    if ( glfwGetKey( window, GLFW_KEY_2 ) == GLFW_PRESS )
        glUniform1i( 1, 1 );

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
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 3, ssbo );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );

    auto vertex_shader = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertex_shader, 1, &vertex_shader_text, NULL );
    compileShader( vertex_shader );
    auto fragment_shader = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragment_shader, 1, &fragment_shader_text, NULL );
    compileShader( fragment_shader );

    auto program = glCreateProgram();
    glAttachShader( program, vertex_shader );
    glAttachShader( program, fragment_shader );
    glLinkProgram( program );
    glUseProgram( program );

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