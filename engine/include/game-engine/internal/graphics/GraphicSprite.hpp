//
// Created by adria on 22/04/2019.
//

#ifndef ZOMBIEWAVE_GRAPHICSPRITE_HPP
#define ZOMBIEWAVE_GRAPHICSPRITE_HPP

#include <game-engine/api.hpp>
#include <SDL_image.h>
#include <game-engine/internal/graphics/MeshData.hpp>
#include <game-engine/internal/graphics/Graphic.hpp>

namespace GameEngine {
    namespace Internal {
        class PUBLICAPI GraphicSprite : public Graphic {
            unsigned int textureID_;
            int width_;
            int height_;
            std::unique_ptr<MeshData> mesh_;
            std::unique_ptr<SDL_Surface> surface_;

        protected:
            virtual void internalInitializeGl();

        public:
            explicit GraphicSprite(const std::string &filename);

            virtual ~GraphicSprite();

            virtual void draw(const std::shared_ptr<Shader> &shader) const;

            virtual int getWidth() const;

            virtual int getHeight() const;

        };
    }
}


#endif //ZOMBIEWAVE_GRAPHICSPRITE_HPP
