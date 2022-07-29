//
// Created by adria on 27/09/2018.
//

#ifndef SPACEINVADERS_BUILDER_HPP
#define SPACEINVADERS_BUILDER_HPP


#include <game-engine/api.hpp>
#include <game-engine/geData.hpp>
#include <memory>
#include <game-engine/components/ComponentData.hpp>
#include <game-engine/geComponent.hpp>

namespace GameEngine {

    class ComponentBuilder;
    
    template<typename ComponentType>
    static ComponentBuilder* CreateComponentBuilder(){
        auto component = std::make_shared<ComponentType>();
        component->setData(component->instantiateData());
        return new ComponentBuilder(component);
    };

    class PUBLICAPI ComponentBuilder {
        geComponentRef _component;
    public:
        explicit ComponentBuilder(const geComponentRef &component) :
            _component(component)
            {};

        geComponentRef Create() {
            const geComponentRef &instance = _component->clone();
            ComponentDataRef compData = instance->instantiateData();
            instance->setData(compData);
            return instance;
        };

        geComponentRef Create(const geData &data) {
            const geComponentRef &instance = _component->clone();
            ComponentDataRef compData = instance->instantiateData();
            compData->fill(data);
            instance->setData(compData);
            return instance;
        };

        ComponentDataRef createProperties()
        {
            return _component->instantiateData();
        };
    };
}

#endif //SPACEINVADERS_BUILDER_HPP
