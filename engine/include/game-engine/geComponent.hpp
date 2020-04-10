//
// Created by adria on 01/02/2019.
//

#ifndef SPACEINVADERS_GECOMPONENT_HPP
#define SPACEINVADERS_GECOMPONENT_HPP

#include <game-engine/api.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/properties/PropertySet.hpp>
#include <game-engine/properties/PropertiesHolder.hpp>
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
        virtual void preInit(){};
        void gameObject(geGameObject *gameObject);
        geGameObject *gameObject() const;
        virtual geComponentRef clone() const = 0;
        virtual PropertySetBase *getProperties() const = 0;
    };

    /**
     * This is a helper class that implement the boiler plate code to instantiate a class derived from \class geComponent.
     * This is only for the first inheritance level, if you derive a class currently derived from \class geComponentInstantiable,
     * you must implement the methods that this class implements: instantiate, clone and getProperties.
     * */
    template<typename T>
    class PUBLICAPI geComponentInstantiable : public geComponent, public PropertiesHolder
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
            compCloned->copyProperties<T>(thisRef);

            return cloned;
        }

    public:
        virtual ~geComponentInstantiable(){};

        virtual PropertySetBase *getProperties() const = 0;
    };
}
#endif //SPACEINVADERS_GECOMPONENT_HPP
