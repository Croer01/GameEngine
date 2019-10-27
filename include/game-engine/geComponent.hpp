//
// Created by adria on 01/02/2019.
//

#ifndef SPACEINVADERS_GECOMPONENT_HPP
#define SPACEINVADERS_GECOMPONENT_HPP

#include <game-engine/api.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/properties/PropertySet.hpp>
#include <memory>

#define COMPONENT(x) //the #x component has been registered

namespace GameEngine {

    class geComponent;
    typedef std::shared_ptr<geComponent> geComponentRef;

    class PUBLICAPI geComponent {
        GameEngine::geGameObject *gameObject_;
    protected:
        virtual void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {};
        virtual PropertySetBase *instantiateProperties();
        virtual geComponentRef instantiate() const = 0;
    public:
        virtual ~geComponent(){};
        virtual void Update(float elapsedTime){};
        virtual void init(){};
        void gameObject(geGameObject *gameObject);
        geGameObject *gameObject() const;
        virtual geComponentRef clone() const = 0;
    };

    template<typename T>
    class PUBLICAPI geComponentInstantiable : public geComponent, public PropertiesHolder<T>
    {
    protected:
        geComponentRef instantiate() const override
        {
            auto instance = std::make_shared<T>();
            return instance;
        }

        virtual geComponentRef clone() const {
            geComponentRef cloned = instantiate();
            auto compCloned = std::dynamic_pointer_cast<T>(cloned);
            auto other = shared_from_this();
            auto thisRef = std::dynamic_pointer_cast<const T>(other);
            compCloned->copyProperties(thisRef);

            return cloned;
        }
    };
}
#endif //SPACEINVADERS_GECOMPONENT_HPP
