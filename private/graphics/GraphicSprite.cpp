//
// Created by adria on 22/04/2019.
//

#include <SDL2/SDL_image.h>
#include "GraphicSprite.hpp"
#include "../utils.hpp"

namespace GameEngine {
    namespace Internal {
        GraphicSprite::GraphicSprite(const std::string &filename) {
            // Load texture
            textureID_ = 0;
            // You should probably use CSurface::OnLoad ... ;)
            //-- and make sure the Surface pointer is good!
            //    SDL_Surface* Surface = IMG_Load(filename);
            SDL_Surface *surface = IMG_Load(filename.c_str());
            CheckSDLError();
            width_ = surface->w;
            height_ = surface->h;

            glGenTextures(1, &textureID_);
            glBindTexture(GL_TEXTURE_2D, textureID_);

            GLenum Mode = GL_RGB;
            if (surface->format->BytesPerPixel == 4) {
                Mode = GL_RGBA;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, Mode, surface->w, surface->h, 0, Mode, GL_UNSIGNED_BYTE, surface->pixels);

            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            // set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            //force draw hard edges (pixels)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            CheckGlError();

            SDL_FreeSurface(surface);
            CheckSDLError();

            // use this static variable to avoid create multiple instances of the same mesh
            static std::shared_ptr<MeshData> meshInstance;
            if(!meshInstance) {
                //The order of the vertices are fliped to from "n" to "u" way to deal with the inverted y axis
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

                meshInstance = std::make_shared<MeshData>(vertices, indices);
            }
            mesh_ = meshInstance;
        }

        GraphicSprite::~GraphicSprite() {
            glDeleteTextures(1, &textureID_);
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
