//
// Created by adria on 12/10/2018.
//

#ifndef SPACEINVADERS_GRAPHIC_HPP
#define SPACEINVADERS_GRAPHIC_HPP


#include <game-engine/api.hpp>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <game-engine/internal/graphics/Shader.hpp>
namespace GameEngine {
namespace Internal {
    class PUBLICAPI Graphic {
        bool initialized_;
    protected:
        virtual void internalInitializeGl() = 0;
    public:
        Graphic() : initialized_(false)
        {}

        virtual ~Graphic() {};

        void initializeGl()
        {
            if(initialized_)
                return;

            internalInitializeGl();
            initialized_ = true;
        }

        virtual void draw(const std::shared_ptr<Shader> &shader) const = 0;

        virtual int getWidth() const = 0;

        virtual int getHeight() const = 0;
    };
}
}

#endif //SPACEINVADERS_GRAPHIC_HPP
