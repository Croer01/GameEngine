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
#include <game-engine/geComponent.hpp>

namespace GameEngine {
namespace Internal {

    class ObjectManager;

    class PUBLICAPI GameObject : public geGameObject, private Observer<GameObjectEvent>, public std::enable_shared_from_this<GameObject> {
        static int ID_GENERATOR;
        int id_;
        bool active_;
        bool activeValueToSetInSafeMode_;
        bool hasActiveValueToSetInSafeMode_;
        std::string prototype_;
        std::string name_;
        std::vector<std::shared_ptr<GameObject>> children_;
        Vec2D position_;
        float rotation_;
        Vec2D scale_;
        std::weak_ptr<GameObject> parent_;
        glm::mat4 transform_;
        bool initializating_;
        bool preInitializating_;
        Game *game_;

        void fromYamlNode(const YAML::Node &node, ObjectManager *objectManager);

    public:

        explicit GameObject(const std::string &prototype);
        virtual ~GameObject();

        void Init();
        void preInit();

        void Update(float elapsedTime);

        std::string getType() const { return prototype_; };

        void addComponent(const geComponentRef &component);

        void addChild(const std::shared_ptr<GameObject>& child);

        void removeChild(const std::shared_ptr<GameObject>& child);

        virtual GameEngine::geGameObjectRef findChildByName(const std::string &name);

        void fromFile(const std::string &filename, ObjectManager *objectManager);

        std::shared_ptr<GameObject> Clone(Game *game) const;

        void parent(geGameObject *parent) override;
        virtual geGameObject *parent() const;

        virtual Vec2D localPosition() const;
        Vec2D position() const override;

        void position(const Vec2D &position) override;

        float rotation() const override;

        void rotation(float rotation) override;

        Vec2D scale() const override;

        void scale(const Vec2D &scale) override;

        glm::mat4 getTransform();

        virtual bool active() const;

        virtual void active(bool active);

        std::string name() const override;
        void name(const std::string &name) override;

        virtual Game *game() const;
        void game(Game *value);

    private:
        void onEvent(const Subject<GameObjectEvent> &target, GameObjectEvent args) override;

        void computeTransform();
    };
}
}

#endif //SPACEINVADERS_GAMEOBJECT_HPP
