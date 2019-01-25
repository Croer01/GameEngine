//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_GAMEOBJECT_HPP
#define SPACEINVADERS_GAMEOBJECT_HPP


#include <string>
#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <yaml-cpp/yaml.h>
#include <geGameObject.hpp>
#include "events/Subject.hpp"
#include "api.hpp"

namespace GameEngine {
namespace Internal {
    class Component;


    class PUBLICAPI GameObject : public geGameObject, public Subject<GameObjectEvent>, private Observer<GameObjectEvent> {
        static int ID_GENERATOR;
        int id_;
        bool active_;
        bool activeValueToSetInSafeMode_;
        bool hasActiveValueToSetInSafeMode_;
        std::string nameType_;
        std::string name_;
        std::vector<std::shared_ptr<Component>> components_;
        std::vector<std::shared_ptr<GameObject>> children_;
        Vec2D position_;
        Vec2D rotation_;
        Vec2D scale_;
        GameObject *parent_;
        glm::mat4 transform_;

        void fromYamlNode(const YAML::Node &node);

    public:

        explicit GameObject(const std::string &name);
        ~GameObject();

        void Init();

        void Update(float elapsedTime);

        std::string getType() const { return nameType_; };

        void addComponent(std::shared_ptr<Component> component);

        void addChild(std::shared_ptr<GameObject> child);

        std::shared_ptr<GameObject> findChildByName(const std::string &name);

        void setParent(GameObject *parent);

        void fromFile(const std::string &filename);

        std::shared_ptr<GameObject> Clone() const;

        Vec2D position() const override;

        void position(const Vec2D &position) override;

        Vec2D rotation() const override;

        void rotation(const Vec2D &rotation) override;

        Vec2D scale() const override;

        void scale(const Vec2D &scale) override;

        glm::mat4 getTransform();

        bool isActive() const;

        void setActive(bool active);

        std::string getName() const;
        void setName(const std::string &name);

        template <typename T>
        std::weak_ptr<T> getComponent() const{
            for(auto component : components_){
                if(auto desiredComponent = std::dynamic_pointer_cast<T>(component))
                    return desiredComponent;
            }

            return std::shared_ptr<T>();
        }

    private:
        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

        void computeTransform();
    };
}
}

#endif //SPACEINVADERS_GAMEOBJECT_HPP
