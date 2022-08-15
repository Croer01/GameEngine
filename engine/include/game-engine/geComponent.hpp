//
// Created by adria on 01/02/2019.
//

#ifndef SPACEINVADERS_GECOMPONENT_HPP
#define SPACEINVADERS_GECOMPONENT_HPP

#include <game-engine/api.hpp>
#include <memory>
#include <game-engine/components/ComponentData.hpp>

#define COMPONENT(x) //the #x component has been registered

namespace GameEngine {

    class geComponent;

    typedef std::shared_ptr<geComponent> geComponentRef;

    class geGameObject;

    class PUBLICAPI geComponent {
        GameEngine::geGameObject *gameObject_;
        std::shared_ptr<ComponentData> componentData_;
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
        virtual ComponentDataRef instantiateData() const = 0;
        void setData(const std::shared_ptr<ComponentData> data)
        {
            componentData_ = data;
        };
        ComponentData *getData() const
        {
            return componentData_.get();
        };

        // HELPER METHODS

        template<typename ValueType>
        void setPropertyValue(const std::string &name, const ValueType &value)
        {
            ComponentData *data = getData();
            assert(data);
            data->template getProperty<ValueType>(name)->set(value);
        }

        template<typename ValueType>
        ValueType &getPropertyValue(const std::string &name) const
        {
            ComponentData *data = getData();
            assert(data);
            return data->template getProperty<ValueType>(name)->get();
        }

        template<typename ValueType>
        void setPropertyObserver(const std::string &name, const std::function<void()> &callback)
        {
            ComponentData *data = getData();
            assert(data);
            data->template getProperty<ValueType>(name)->registerObserver(callback);
        }

        void setFilePathPropertyValue(const std::string &name, const std::string &value)
        {
            ComponentData *data = getData();
            assert(data);
            data->getFilePathProperty(name)->set(value);
        }

        std::string &getFilePathPropertyValue(const std::string &name) const
        {
            ComponentData *data = getData();
            assert(data);
            return data->getFilePathProperty(name)->get();
        }

        void setFilePathPropertyObserver(const std::string &name, const std::function<void()> &callback)
        {
            ComponentData *data = getData();
            assert(data);
            data->getFilePathProperty(name)->registerObserver(callback);
        }

        void setEnumPropertyValue(const std::string &name, const std::string &value)
        {
            ComponentData *data = getData();
            assert(data);
            data->getEnumProperty(name)->set(value);
        }

        std::string &getEnumPropertyValue(const std::string &name) const
        {
            ComponentData *data = getData();
            assert(data);
            return data->getEnumProperty(name)->get();
        }

        void setEnumPropertyObserver(const std::string &name, const std::function<void()> &callback)
        {
            ComponentData *data = getData();
            assert(data);
            data->getEnumProperty(name)->registerObserver(callback);
        }
    };

    /**
     * This is a helper class that implement the boiler plate code to instantiate a class derived from \class geComponent.
     * This is only for the first inheritance level, if you derive a class currently derived from \class geComponentInstantiable,
     * you must implement the methods as this class implements: instantiate, clone and instantiateData.
     * */
    
    #define COMPONENT_BOILERPLATE(CompT, CompDataT)                   \
    protected:                                                        \
        GameEngine::geComponentRef instantiate() const override       \
        {                                                             \
            auto instance = std::make_shared<CompT>();                \
            return instance;                                          \
        };                                                            \
                                                                      \
        GameEngine::geComponentRef clone() const override {           \
            GameEngine::geComponentRef cloned = instantiate();        \
            cloned->setData(getData()->clone<CompDataT>());           \
            return cloned;                                            \
        };                                                            \
                                                                      \
    public:                                                           \
        GameEngine::ComponentDataRef instantiateData() const override \
        {                                                             \
            auto instance = std::make_shared<CompDataT>();            \
            return instance;                                          \
        };                                                            \
    private:


    template<typename CompT, typename CompDataT>
    class PUBLICAPI geComponentInstantiable : public geComponent
    {
        COMPONENT_BOILERPLATE(CompT, CompDataT)
    public:
        virtual ~geComponentInstantiable(){};
    };
}
#endif //SPACEINVADERS_GECOMPONENT_HPP
