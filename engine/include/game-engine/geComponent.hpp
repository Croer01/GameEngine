//
// Created by adria on 01/02/2019.
//

#ifndef SPACEINVADERS_GECOMPONENT_HPP
#define SPACEINVADERS_GECOMPONENT_HPP

#include <game-engine/api.hpp>
#include <memory>
#include <game-engine/geGameObject.hpp>
#include <game-engine/components/ComponentData.hpp>

#define COMPONENT(x) //the #x component has been registered

namespace GameEngine {

    class geComponent;

    typedef std::shared_ptr<geComponent> geComponentRef;

    class PUBLICAPI geComponent {
        GameEngine::geGameObject *gameObject_;
    protected:
        virtual void onGameObjectChange(GameEngine::geGameObject *oldGameObject, GameEngine::geGameObject *newGameObject) {};
        virtual geComponentRef instantiate() const = 0;
    public:
        virtual ~geComponent(){};
        virtual void Update(float elapsedTime){};
        virtual void init(){};
        virtual void preInit(){};
        void gameObject(geGameObject *gameObject);
        geGameObject *gameObject() const;
        virtual geComponentRef clone() const = 0;
        virtual void setData(const std::shared_ptr<ComponentData> data) = 0;
        virtual ComponentData *getData() const = 0;
        virtual ComponentDataRef instantiateData() const = 0;
    };

    /**
     * This is a helper class that implement the boiler plate code to instantiate a class derived from \class geComponent.
     * This is only for the first inheritance level, if you derive a class currently derived from \class geComponentInstantiable,
     * you must implement the methods as this class implements: instantiate, clone and getProperties.
     * */
    template<typename CompT, typename CompDataT>
    class PUBLICAPI geComponentInstantiable : public geComponent
    {
        std::shared_ptr<ComponentData> componentData_;
    protected:
        geComponentRef instantiate() const override
        {
            auto instance = std::make_shared<CompT>();
            return instance;
        }

        virtual geComponentRef clone() const {
            geComponentRef cloned = instantiate();
            cloned->setData(getData()->template clone<CompDataT>());

            return cloned;
        }

    public:
        virtual ~geComponentInstantiable(){};

        virtual void setData(const std::shared_ptr<ComponentData> data)
        {
            componentData_ = data;
        };

        virtual ComponentData *getData() const
        {
            return componentData_.get();
        };

        virtual ComponentDataRef instantiateData() const
        {
            auto instance = std::make_shared<CompDataT>();
            return instance;
        }

        template<typename ValueType>
        void setPropertyValue(const std::string &name, const ValueType &value)
        {
            assert(componentData_);
            componentData_->template getProperty<ValueType>(name)->set(value);
        }

        template<typename ValueType>
        ValueType &getPropertyValue(const std::string &name) const
        {
            assert(componentData_);
            return componentData_->template getProperty<ValueType>(name)->get();
        }

        template<typename ValueType>
        void setPropertyObserver(const std::string &name, const std::function<void()> &callback)
        {
            assert(componentData_);
            componentData_->template getProperty<ValueType>(name)->registerObserver(callback);
        }

        void setFilePathPropertyValue(const std::string &name, const std::string &value)
        {
            assert(componentData_);
            componentData_->getFilePathProperty(name)->set(value);
        }

        std::string &getFilePathPropertyValue(const std::string &name) const
        {
            assert(componentData_);
            return componentData_->getFilePathProperty(name)->get();
        }

        void setFilePathPropertyObserver(const std::string &name, const std::function<void()> &callback)
        {
            assert(componentData_);
            componentData_->getFilePathProperty(name)->registerObserver(callback);
        }

        void setEnumPropertyValue(const std::string &name, const std::string &value)
        {
            assert(componentData_);
            componentData_->getEnumProperty(name)->set(value);
        }

        std::string &getEnumPropertyValue(const std::string &name) const
        {
            assert(componentData_);
            return componentData_->getEnumProperty(name)->get();
        }

        void setEnumPropertyObserver(const std::string &name, const std::function<void()> &callback)
        {
            assert(componentData_);
            componentData_->getEnumProperty(name)->registerObserver(callback);
        }
    };
}
#endif //SPACEINVADERS_GECOMPONENT_HPP
