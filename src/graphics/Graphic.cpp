//
// Created by adria on 12/10/2018.
//

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include "Graphic.hpp"
#include "../utils.hpp"

Graphic::Graphic(std::string filename) {
    modelTransform_ = glm::mat4(1);
    // Load texture
    textureID_ = 0;
    // You should probably use CSurface::OnLoad ... ;)
    //-- and make sure the Surface pointer is good!
    //    SDL_Surface* Surface = IMG_Load(filename);
    SDL_Surface *surface = IMG_Load(filename.c_str());

    CheckSDLError();

    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);

    GLenum Mode = GL_RGB;
    if (surface->format->BytesPerPixel == 4) {
        Mode = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, Mode, surface->w, surface->h, 0, Mode, GL_UNSIGNED_BYTE, surface->pixels);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    CheckGlError();

    SDL_FreeSurface(surface);
}

Graphic::~Graphic() {
    glDeleteTextures(0, &textureID_);
}

void Graphic::draw(std::shared_ptr<Shader> shader) {
    glBindTexture(GL_TEXTURE_2D, textureID_);

    shader->setUniform("transform", modelTransform_);
    shader->draw();

    CheckGlError();
}

void Graphic::setModelTransform(const glm::mat4 &transform) {
    modelTransform_ = transform;
}