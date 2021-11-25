
#include "Engine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include<iostream>
#include<string>
///*****************************************************************************
//
// Our main function
int main(int argc, char *argv[]) {	
	
    auto engine = new Engine(4, 1, 640, 480, "A4: Off to Never Never Land");
	engine->initialize();
    if (engine->getError() == CSCI441::OpenGLEngine::OPENGL_ENGINE_ERROR_NO_ERROR) {
        engine->run();
    }
    engine->shutdown();
    delete engine;

	return EXIT_SUCCESS;
}
