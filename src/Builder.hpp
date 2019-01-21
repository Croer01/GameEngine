//
// Created by adria on 27/09/2018.
//

#ifndef SPACEINVADERS_BUILDER_HPP
#define SPACEINVADERS_BUILDER_HPP


#include "Component.hpp"
#include <memory>
namespace GameEngine {
namespace Internal {
    class ComponentBuilder {
    public:
        virtual ~ComponentBuilder() = default;

        virtual std::shared_ptr<Component> Create() = 0;
    };

    template<typename ComponentType>
    class ComponentTBuilder : public ComponentBuilder {
    public:
        virtual std::shared_ptr<Component> Create() {
            return std::make_shared<ComponentType>();
        };
    };
}
}

#endif //SPACEINVADERS_BUILDER_HPP
