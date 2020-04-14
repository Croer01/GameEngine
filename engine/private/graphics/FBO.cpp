//
// Created by adria on 12/04/2020.
//

#include <GL/glew.h>
#include <stdexcept>
#include "FBO.hpp"
#include "../utils.hpp"

namespace GameEngine{
namespace Internal{
FBO::FBO(int width, int height)
{
    glGenFramebuffers(1, &fbo_);

    glGenTextures(1, &textureAttatchment_);
    glBindTexture(GL_TEXTURE_2D, textureAttatchment_);

    // generate empty texture. This will be filled by FBO
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // attach the texture to the fbo
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureAttatchment_, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Unexpected error creating FBO");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBO::~FBO()
{
    // ensure we unbind the frame buffer before delete it
    unBind();
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(1, &textureAttatchment_);
    CheckGlError();
}

void FBO::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    CheckGlError();
}

void FBO::unBind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CheckGlError();
}

unsigned int FBO::getTextId() const
{
    return textureAttatchment_;
}

}
}
