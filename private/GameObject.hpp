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
#include <game-engine/geGameObject.hpp>
#include <game-engine/api.hpp>
#include <game-engine/geComponent.hpp>

namespace GameEngine {
namespace Internal {

    class GameObject : public geGameObject, private Observer<GameObjectEvent>, public std::enable_shared_from_this<GameObject> {
        static int ID_GENERATOR;
        int id_;
        bool active_;
        bool activeValueToSetInSafeMode_;
        bool hasActiveValueToSetInSafeMode_;
        std::string prototype_;
        std::string name_;
        std::vector<std::shared_ptr<GameObject>> children_;
        Vec2D position_;
        Vec2D rotation_;
        Vec2D scale_;
        std::weak_ptr<GameObject> parent_;
        glm::mat4 transform_;
        bool destroyed_;

        void fromYamlNode(const YAML::Node &node);

    public:

        explicit GameObject(const std::string &prototype);
        virtual ~GameObject();

        void Init();

        void Update(float elapsedTime);

        std::string getType() const { return prototype_; };

        void addComponent(const geComponentRef &component);

        void addChild(std::shared_ptr<GameObject> child);

        virtual GameEngine::geGameObjectRef findChildByName(const std::string &name);

        void fromFile(const std::string &filename);

        std::shared_ptr<GameObject> Clone() const;

        void parent(const geGameObjectRef &parent) override;

        Vec2D position() const override;

        void position(const Vec2D &position) override;

        Vec2D rotation() const override;

        void rotation(const Vec2D &rotation) override;

        Vec2D scale() const override;

        void scale(const Vec2D &scale) override;

        glm::mat4 getTransform();

        virtual bool active() const;

        virtual void active(bool active);

        std::string name() const override;
        void name(const std::string &name) override;

        virtual geGame &game() const;

        virtual bool isDestroyed() const;
        virtual void destroy();

    private:
        void onEvent(const Subject<GameObjectEvent> &target, const GameObjectEvent &event, void *args) override;

        void computeTransform();
    };
}
}

#endif //SPACEINVADERS_GAMEOBJECT_HPP
