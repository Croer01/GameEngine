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
#include "events/Subject.hpp"
#include "api.hpp"

namespace GameEngine {
namespace Internal {
    class Component;

    enum class GameObjectEvent{
        TransformChanged,
        PositionChanged,
        RotationChanged,
        ScaleChanged,
        ActiveChanged
    };

    class GameObject : public GameEngine::GameObject, public Subject<GameObjectEvent>, private Observer<GameObjectEvent> {
        static int ID_GENERATOR;
        int id_;
        bool active_;
        bool activeValueToSetInSafeMode_;
        bool hasActiveValueToSetInSafeMode_;
        std::string nameType_;
        std::string name_;
        std::vector<std::shared_ptr<Component>> components_;
        std::vector<std::shared_ptr<GameObject>> children_;
        glm::vec3 position_;
        glm::vec3 rotation_;
        glm::vec3 scale_;
        GameObject *parent_;

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

        glm::vec3 getPosition() const;

        void setPosition(const glm::vec3 &position);

        glm::vec3 getRotation() const;

        void setRotation(const glm::vec3 &rotation);

        glm::vec3 getScale() const;

        void setScale(const glm::vec3 &scale);

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

        Vec3D position() const override;

        void position(const Vec3D &position) override;

        Vec3D rotation() const override;

        void rotation(const Vec3D &rotation) override;

        Vec3D scale() const override;

        void scale(const Vec3D &scale) override;

    private:
        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;
    };
}
}

#endif //SPACEINVADERS_GAMEOBJECT_HPP
