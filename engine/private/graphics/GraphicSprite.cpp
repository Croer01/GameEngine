//
// Created by adria on 22/04/2019.
//

#include <SDL2/SDL_image.h>
#include "GraphicSprite.hpp"
#include "../utils.hpp"

namespace GameEngine {
    namespace Internal {
    GraphicSprite::GraphicSprite(const std::string &filename)
    {
        // Load texture
        textureID_ = -1;
        // You should probably use CSurface::OnLoad ... ;)
        //-- and make sure the Surface pointer is good!
        //    SDL_Surface* Surface = IMG_Load(filename);
        surface_.reset(IMG_Load(filename.c_str()));
        CheckSDLError();
        width_ = surface_->w;
        height_ = surface_->h;
    }

    void GraphicSprite::initializeGl()
    {
        glGenTextures(1, &textureID_);
        glBindTexture(GL_TEXTURE_2D, textureID_);

        CheckGlError();

        GLenum Mode = GL_RGB;
        if (surface_->format->BytesPerPixel == 4) {
            Mode = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, Mode, surface_->w, surface_->h, 0, Mode, GL_UNSIGNED_BYTE, surface_->pixels);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //force draw hard edges (pixels)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        CheckGlError();

        SDL_FreeSurface(surface_.release());
        CheckSDLError();

        //The order of the vertices are flipped to from "n" to "u" way to deal with the inverted y axis
        //VBO data
        std::vector<float> vertices = std::vector<float>({ //vertex(3) | uv(2)
                                                             0.f, 1.f, 0.f, 0.f, 1.f,
                                                             0.f, 0.f, 0.f, 0.f, 0.f,
                                                             1.f, 0.f, 0.f, 1.f, 0.f,
                                                             1.f, 1.f, 0.f, 1.f, 1.f
                                                         });
        //IBO data
        std::vector<unsigned int> indices = std::vector<unsigned int>({
                                                                          0, 1, 2,
                                                                          0, 2, 3
                                                                      });

        mesh_ = std::make_unique<MeshData>(vertices, indices);
    }

        GraphicSprite::~GraphicSprite()
        {
            if(surface_)
                SDL_FreeSurface(surface_.release());

            if(textureID_ != -1)
            {
                glDeleteTextures(1, &textureID_);
                textureID_ = -1;
            }
        }

        void GraphicSprite::draw(const std::shared_ptr<Shader> &shader) const {
            glBindTexture(GL_TEXTURE_2D, textureID_);

            mesh_->draw(shader);
            shader->draw();
            CheckGlError();
        }

        int GraphicSprite::getWidth() const {
            return width_;
        }

        int GraphicSprite::getHeight() const {
            return height_;
        }
    }
}
