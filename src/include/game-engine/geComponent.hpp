//
// Created by adria on 01/02/2019.
//

#ifndef SPACEINVADERS_GECOMPONENT_HPP
#define SPACEINVADERS_GECOMPONENT_HPP

#include <memory>
#include <game-engine/api.hpp>

namespace GameEngine {

    class geComponent;
    typedef std::shared_ptr<geComponent> geComponentRef;

    class geGameObject;
    class PUBLICAPI geComponent{
    public:
        virtual ~geComponent() = default;
        virtual void Update(float elapsedTime) = 0;
        virtual void init() = 0;
        virtual void gameObject(geGameObject *gameObject) = 0;
        virtual geGameObject *gameObject() const = 0;
    };
}
#endif //SPACEINVADERS_GECOMPONENT_HPP
