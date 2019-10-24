//
// Created by adria on 27/09/2018.
//

#ifndef SPACEINVADERS_BUILDER_HPP
#define SPACEINVADERS_BUILDER_HPP


#include <game-engine/geData.hpp>
#include <memory>

namespace GameEngine {

    class geComponent;
    typedef std::shared_ptr<geComponent> geComponentRef;

    class ComponentBuilder {
    public:
        virtual ~ComponentBuilder() = default;

        virtual geComponentRef Create(const geData &data) = 0;
    };

    template<typename ComponentType>
    class ComponentTBuilder : public ComponentBuilder {
    public:
        virtual geComponentRef Create(const geData &data) {

            const std::shared_ptr<ComponentType> &instance = std::make_shared<ComponentType>();
            instance->copy(data);
            return instance;
        };
    };
}

#endif //SPACEINVADERS_BUILDER_HPP
