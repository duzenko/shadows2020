#include <glad/glad.h>
#include "common.h"
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <cmath>
#include <fstream>
#include "GlfwApp.h"
#include "..\glhelpers\GlProgram.h"

int main() {
    GlfwApp app;
    GlProgram shader;
    while ( app.run() ) {
        shader.limit = app.keyL;
        
        float viewAngle = atan( (float) app.SCR_HEIGHT / app.SCR_WIDTH ) * 2;
        glm::mat4 proj = glm::perspective( viewAngle, (float) app.SCR_WIDTH / app.SCR_HEIGHT, .1f, 10.f );
        auto projInv = glm::inverse( proj );
        auto screenPos = glm::vec4( app.xpos / app.SCR_WIDTH * 2 - 1, 1 - 2 * app.ypos / app.SCR_HEIGHT, 0, 0 );
        auto worldPos = projInv * screenPos;
        shader.lightPos = glm::vec2( worldPos );

        shader.matProjection = proj;

        glm::mat4 view = glm::lookAt(
            glm::vec3( 0, 0, 1 ),
            glm::vec3( 0, 0, 0 ),
            glm::vec3( 0, 1, 0 )
        );
        shader.matView = view;

        shader.soften = app.keyS;

        bool sendTime = app.keyT;
        if ( sendTime ) {
            shader.time = (float) fmod( app.time, 1 );
        }

        shader.lightSize = app.scrollY;
        
        draw();
    }
    return 0;
}
