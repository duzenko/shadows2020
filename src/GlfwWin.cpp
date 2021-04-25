#include "pch.h"

#include <GLFW/glfw3.h>

GLFWwindow* window;

void framebuffer_size_callback( GLFWwindow* window, int width, int height ) {
    glViewport( 0, 0, width, height );
}

void key_callback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    if ( action != GLFW_PRESS ) {
        return;
    }
    GlfwWin &app = *(GlfwWin*) glfwGetWindowUserPointer( window );
    if ( key == GLFW_KEY_L ) {
        app.keyL = !app.keyL;
    }
    if ( key == GLFW_KEY_S ) {
        app.keyS = app.keyS ? app.keyS * 2 % 8 : 1;
    }
    if ( key == GLFW_KEY_T ) {
        app.keyT = !app.keyT;
    }
}

void scroll_callback( GLFWwindow* window, double xoffset, double yoffset ) {
    GlfwWin& app = *(GlfwWin*) glfwGetWindowUserPointer( window );
    app.scrollY *= exp2f( (float) yoffset * 2e-1f );
}

void GlfwWin::processInput() {
    if ( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
        glfwSetWindowShouldClose( window, true );

    glfwGetCursorPos( window, &xpos, &ypos );
    glfwGetFramebufferSize( window, &SCR_WIDTH, &SCR_HEIGHT );
    time = glfwGetTime();
}

GlfwWin::GlfwWin() {
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

    //glfwSwapInterval( 1 );
    glEnable( GL_DEPTH_TEST );
}

GlfwWin::~GlfwWin() {
    glfwTerminate();
}

bool GlfwWin::messageLoop() {
    if ( !window )
        return false;
    processInput();
    glfwSwapBuffers( window );
    glfwPollEvents();
    return !glfwWindowShouldClose( window );
}