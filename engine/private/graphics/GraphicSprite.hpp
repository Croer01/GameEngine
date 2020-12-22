//
// Created by adria on 22/04/2019.
//

#ifndef ZOMBIEWAVE_GRAPHICSPRITE_HPP
#define ZOMBIEWAVE_GRAPHICSPRITE_HPP

#include <SDL_image.h>
#include "MeshData.hpp"
#include "Graphic.hpp"

namespace GameEngine {
    namespace Internal {
        class GraphicSprite : public Graphic {
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
