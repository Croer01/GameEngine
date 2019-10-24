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

    class PUBLICAPI geComponent{
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
        virtual void copy(const geData& data) = 0;
    };

    template<typename T>
    class PUBLICAPI geComponentInstantiable : public geComponent
    {
        static std::shared_ptr<PropertySetBase> PROPERTIES;
        std::unique_ptr<PropertiesBinderBase> properties_;

    public:
        virtual void copy(const geData& data)
        {
            properties_->fillFrom(data);
        }
    protected:
        geComponentRef instantiate() const override
        {
            auto instance = std::make_shared<T>();
            auto compInstance = std::dynamic_pointer_cast<geComponentInstantiable<T>>(instance);
            auto propertiesInstance = std::dynamic_pointer_cast<PropertySet<T>>(PROPERTIES);
            PropertiesBinderBase *binding = new PropertiesBinder<T>(instance, propertiesInstance);
            compInstance->properties_ = std::unique_ptr<PropertiesBinderBase>(binding);
            return instance;
        }
        virtual PropertySetBase *instantiateProperties()
        {
            return new PropertySet<T>();
        }

        virtual std::weak_ptr<PropertySetBase> getProperties()
        {
            if(!PROPERTIES)
                PROPERTIES.reset(instantiateProperties());

            return PROPERTIES;
        }

        virtual geComponentRef clone() const {
            geComponentRef cloned = instantiate();
            auto compCloned = std::dynamic_pointer_cast<T>(cloned);

            auto properties = dynamic_cast<PropertiesBinder<T> *>(properties_.get());
            properties->fillFrom(compCloned);

            return cloned;
        }
    };

template<typename T>
std::shared_ptr<PropertySetBase> geComponentInstantiable<T>::PROPERTIES;
}
#endif //SPACEINVADERS_GECOMPONENT_HPP
