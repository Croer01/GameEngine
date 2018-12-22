//
// Created by adria on 19/08/2018.
//

#include <exception>
#include <string>
#include <gl/glew.h>
#include <iostream>
#include <SDL2/SDL_error.h>
#include <Al/al.h>
#include "utils.hpp"


void _CheckGlError(const char *file, int line) {
    GLenum err(glGetError());

    while (err != GL_NO_ERROR) {
        std::string error;

        switch (err) {
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }

        std::cerr << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
        err = glGetError();
    }
}

void _CheckSDLError(int line) {
    std::string error = SDL_GetError();

    if (!error.empty()) {
        std::cerr << "SLD Error : " << error << std::endl;

        if (line != -1)
            std::cerr << "\nLine : " << line << std::endl;

        SDL_ClearError();
        throw std::runtime_error(error);
    }
}

void _CheckAlError(const char *file, int line) {
    ALenum err(alGetError());

    while (err != AL_NO_ERROR) {
        std::string error;

        switch (err) {
            case AL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case AL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case AL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case AL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case AL_INVALID_NAME:
                error = "INVALID_NAME";
                break;
        }

        std::cerr << "AL_" << error.c_str() << " - " << file << ":" << line << std::endl;
        err = alGetError();
    }
}
