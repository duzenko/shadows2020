// glhelpers.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "glhelpers.h"

#include <cmath>

#define glerr(X) {X, #X}

std::map<GLint, const char*> glErrors = {
    glerr( GL_INVALID_OPERATION),
};

void checkErrors() {
    auto error = glGetError();
    while ( error != GL_NO_ERROR ) {
        auto errText = glErrors[error];
        if(errText)
            std::cout << "GL error " << std::hex << error << ' ' << errText << '\n';
        else
            std::cout << "GL error " << error << '\n';
        error = glGetError();
    }
}

extern "C" {
    __declspec( dllexport ) int NvOptimusEnablement = 0x00000001;
    __declspec( dllexport ) int AmdPowerXpressRequestHighPerformance = 1;
}

