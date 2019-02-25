//
// Created by adria on 01/02/2019.
//

#ifndef SPACEINVADERS_GECOMPONENT_HPP
#define SPACEINVADERS_GECOMPONENT_HPP

#include <memory>
#include <game-engine/api.hpp>
#include <game-engine/properties/PropertySet.hpp>

#define COMPONENT(x) //the #x component has been registered

namespace GameEngine {

    class geComponent;
    typedef std::shared_ptr<geComponent> geComponentRef;

    class geGameObject;
    class PUBLICAPI geComponent{
        GameEngine::geGameObject *gameObject_;
        PropertySetBase* properties_;
    protected:
        virtual void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {};
        virtual PropertySetBase *configureProperties();
        virtual geComponentRef instantiate() const;
    public:
        virtual ~geComponent();

        virtual void Update(float elapsedTime){};
        virtual void init(){};
        void gameObject(geGameObject *gameObject);
        geGameObject *gameObject() const;
        PropertySetBase &properties();
        void properties(PropertySetBase &properties);
        geComponentRef clone() const;
    };

    template<typename T>
    class PUBLICAPI geComponentInstantiable : public geComponent{
    protected:
        geComponentRef instantiate() const override {
            return std::make_shared<T>();
        }
    };
}
#endif //SPACEINVADERS_GECOMPONENT_HPP
