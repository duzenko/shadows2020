#pragma once

#include "pch.h"

struct App {
	std::vector<glm::vec4> points;
    GlfwWin window;
    GlSsbo ssbo;
    GlPbo pbo;
    ShadowShader shadows;
    ColorShader color;

    App() : shadows( "pass1" ), color( "pass2" ) {
        srand( 0xabcdef );
        glm::vec4 pyramid[9];
        for ( int pyra = 0; pyra < PyramidCount; pyra++ ) {
            randomPyramid( pyramid );
            points.insert( points.end(), pyramid, pyramid + 9);
        }
        ssbo.Upload( points );
    }

    void run() {
        while ( window.messageLoop() ) {
            draw();
        }
    }

    void draw() {
        glClearColor( 1, 1, 1, 1 );
        glEnable( GL_BLEND );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        // depth pass
        glDepthFunc( GL_LESS );
        glDepthMask( GL_TRUE );
        glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
        checkErrors();
        drawGeometry();
        glDepthFunc( GL_EQUAL );
        glDepthMask( GL_FALSE );
        glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
        // shadows pass
        drawPass( shadows );
        pbo.Resize( window.SCR_WIDTH * window.SCR_HEIGHT * 4 );
        if ( !window.mouseMoved && window.keyB ) {
            glBlendColor( 0, 0, 0, 1.0f / ( 2 << 4 ) );
            glBlendFunc( GL_ONE_MINUS_CONSTANT_ALPHA, GL_CONSTANT_ALPHA );
            glDisable( GL_DEPTH_TEST );
            checkErrors();
            glDrawPixels( window.SCR_WIDTH, window.SCR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
            checkErrors();
            glEnable( GL_DEPTH_TEST );
        }
        checkErrors();
        glReadPixels( 0, 0, window.SCR_WIDTH, window.SCR_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
        checkErrors();
        glBlendFunc( GL_DST_COLOR, GL_ZERO );
        checkErrors();
        // color pass
        drawPass( color );
    }

    void drawGeometry( bool colorShader = false ) {
        // canvas
        checkErrors();
        if ( colorShader )
            glUniform1f( 0, 0.3f );
        checkErrors();
        glBegin( GL_QUADS );
        glVertex2i( -1, -1 );
        glVertex2i( 1, -1 );
        glVertex2i( 1, 1 );
        glVertex2i( -1, 1 );
        glEnd();
        checkErrors();
        // pyramids
        if ( colorShader )
            glUniform1f( 0, 1 );
        checkErrors();
        glBegin( GL_TRIANGLES );
        for ( int i = 0; i < VertexCount; i++ ) {
            glVertex3fv( glm::value_ptr( points[i] ) );
        }
        glEnd();
        checkErrors();
    }

    void drawPass( CommonShader& shader ) {
        shader.Use();
        shader.setup( window );
        drawGeometry( shader.name == "pass2" );
    }
};


