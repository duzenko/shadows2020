#include "GlfwApp.h"

#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

void framebuffer_size_callback( GLFWwindow* window, int width, int height ) {
    glViewport( 0, 0, width, height );
}

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    if ( action != GLFW_PRESS ) {
        return;
    }
    GlfwApp &app = *(GlfwApp*) glfwGetWindowUserPointer( window );
    if ( key == GLFW_KEY_L ) {
        app.keyL = !app.keyL;
    }
    if ( key == GLFW_KEY_S ) {
        app.keyS = !app.keyS;
    }
    if ( key == GLFW_KEY_T ) {
        app.keyT = !app.keyT;
    }
}

void scroll_callback( GLFWwindow* window, double xoffset, double yoffset ) {
    GlfwApp& app = *(GlfwApp*) glfwGetWindowUserPointer( window );
    app.scrollY *= exp2f( (float) yoffset * -2e-1f );
}

void GlfwApp::processInput() {
    if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
        glfwSetWindowShouldClose( window, true );

    glfwGetCursorPos( window, &xpos, &ypos );
    glfwGetFramebufferSize( window, &SCR_WIDTH, &SCR_HEIGHT );
    time = glfwGetTime();
}

GlfwApp::GlfwApp() {
    glfwInit();
    glfwWindowHint( GLFW_MAXIMIZED, GLFW_TRUE );
    window = glfwCreateWindow( SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL );
    if ( window == NULL ) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwSetWindowUserPointer( window, this );
    glfwMakeContextCurrent( window );
    glfwSetFramebufferSizeCallback( window, framebuffer_size_callback );
    glfwSetKeyCallback( window, key_callback );
    glfwSetScrollCallback( window, scroll_callback );

    if ( !gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ) ) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        window = NULL;
        return;
    }

    glfwSwapInterval( 1 );

    init();

    GLuint ssbo;
    glGenBuffers( 1, &ssbo );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, ssbo );
    glBufferData( GL_SHADER_STORAGE_BUFFER, ssboSize, ssboData, GL_DYNAMIC_COPY );
    glBindBufferBase( GL_SHADER_STORAGE_BUFFER, 0, ssbo );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
}

GlfwApp::~GlfwApp() {
    glfwTerminate();
}

bool GlfwApp::run() {
    if ( !window )
        return false;
    processInput();
    glfwSwapBuffers( window );
    glfwPollEvents();
    return !glfwWindowShouldClose( window );
}