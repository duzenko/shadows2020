// glhelpers.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "glhelpers.h"

#include <cmath>

extern "C" {
    __declspec( dllexport ) int NvOptimusEnablement = 0x00000001;
    __declspec( dllexport ) int AmdPowerXpressRequestHighPerformance = 1;
}

