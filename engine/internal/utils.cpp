//
// Created by adria on 19/08/2018.
//

#include <exception>
#include <string>
#include <GL/glew.h>
#include <iostream>
#include <SDL2/SDL_error.h>
#include <AL/al.h>
#include <sstream>
#include <game-engine/internal/utils.hpp>

namespace GameEngine {
namespace Internal {

    void _CheckGlError(const char *file, int line) {
#ifdef DEBUG
        GLenum err = glGetError();
        bool haveErrors = false;
        while (err != GL_NO_ERROR) {

            std::stringstream buffer;

            buffer << "GL_" << glewGetErrorString(err) << " - " << file << ":" << line << std::endl;
            std::cerr << buffer.rdbuf()->str();
            err = glGetError();
            throw std::runtime_error(buffer.rdbuf()->str());
        }
#endif
    }

    void _CheckSDLError(int line) {
#ifdef DEBUG
        std::string error = SDL_GetError();

        if (!error.empty()) {
            std::cerr << "SLD Error : " << error << std::endl;

            if (line != -1)
                std::cerr << "\nLine : " << line << std::endl;

            SDL_ClearError();
            throw std::runtime_error(error);
        }
#endif
    }

    void _CheckAlError(const char *file, int line) {
#ifdef DEBUG
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
#endif
    }

void CheckFreeTypeError(int errorCode)
{
    if (errorCode)
    {
        std::string errorMsg = "ERROR::FREETYPE: ";
        std::string errorText;
        switch (errorCode)
        {
            case 1:
                errorText = "cannot open resource";
                break;
            default:
                errorText = "Unmapped FreeType error. Code " + std::to_string(errorCode);
        }
        errorMsg += errorText;
        throw std::runtime_error(errorMsg);

    }
}
}
}