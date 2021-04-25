#pragma once

#include "pch.h"

struct CommonShader : GlProgram {
    using GlProgram::GlProgram;

    glm::mat4 proj;

    virtual void setup( GlfwWin& app ) {
        // GLSL routine - projection matrix
        float fovY = atan( (float) app.SCR_HEIGHT / app.SCR_WIDTH ) * 2;
        proj = glm::perspective( fovY, (float) app.SCR_WIDTH / app.SCR_HEIGHT, .1f, 10.f );
        matProjection = proj;

        // GLSL routine - view matrix
        glm::mat4 view = glm::lookAt(
            glm::vec3( 0, 0, 1 ),
            glm::vec3( 0, 0, 0 ),
            glm::vec3( 0, 1, 0 )
        );
        matView = view;
    }
};

struct ShadowShader : CommonShader {

    using CommonShader::CommonShader;

    virtual void setup( GlfwWin& app ) {
        CommonShader::setup( app );
        // light follows mouse cursor
        auto projInv = glm::inverse( proj );
        auto screenPos = glm::vec4( app.xpos / app.SCR_WIDTH * 2 - 1, 1 - 2 * app.ypos / app.SCR_HEIGHT, 0, 0 );
        auto worldPos = projInv * screenPos;
        lightPos = glm::vec2( worldPos );

        // stop time noise
        bool sendTime = app.keyT;
        if ( sendTime ) {
            time = (float) fmod( app.time, 2 * M_PI );
        }

        // light size
        lightSize = app.scrollY;

        // SSBO size info
        vertexCount = VertexCount;

        // speed test - one triangle only
        limit = app.keyL;

        // softener level
        soften = app.keyS;
    }
};

struct ColorShader : CommonShader {
    using CommonShader::CommonShader;
};