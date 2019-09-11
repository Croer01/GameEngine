#include "gtest/gtest.h"
#include "../private/GameObject.hpp"
#include <game-engine/geGame.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/components/SpriteComponent.hpp>

class InitializeCheckComponent : public GameEngine::geComponentInstantiable<InitializeCheckComponent>
{
    bool initialized_;

    void init() override
    {
        initialized_ = true;
    }
public:
    InitializeCheckComponent() :
        initialized_(false)
    { }

    bool getInitialized() const
    {
        return initialized_;
    }
};

class AddOnInitializeComponent : public GameEngine::geComponentInstantiable<AddOnInitializeComponent>
{
    void init() override
    {
        auto component = std::make_shared<InitializeCheckComponent>();
        gameObject()->addComponent(component);
    }
};

TEST(GameObject, createObject)
{
    GameEngine::geGame game;

    GameEngine::geGameObjectRef go = game.createObject("test1");

    ASSERT_EQ(go->name(), "test1");
}

TEST(GameObject, changeName)
{
    const std::string &oldName = "old name";
    const std::string &newName = "new name";
    GameEngine::geGame game;
    GameEngine::geGameObjectRef go = game.createObject(oldName);

    ASSERT_EQ(go->name(), oldName);
    go->name(newName);
    ASSERT_EQ(go->name(), newName);
}


TEST(GameObject, positionChangedByParent)
{
    GameEngine::geGame game;
    GameEngine::geGameObjectRef parent = game.createObject("parent");
    GameEngine::geGameObjectRef child = game.createObject("child");
    const GameEngine::Vec2D &parentPos = GameEngine::Vec2D(5,5);
    const GameEngine::Vec2D &childPos = GameEngine::Vec2D(2,2);
    parent->position(parentPos);
    child->position(childPos);

    ASSERT_EQ(parent->position(), parentPos);
    ASSERT_EQ(child->position(), childPos);

    child->parent(parent);
    ASSERT_EQ(parent->position(), parentPos);
    ASSERT_EQ(child->position(), parentPos + childPos);

    child->parent(nullptr);
    ASSERT_EQ(parent->position(), parentPos);
    ASSERT_EQ(child->position(), childPos);

}

TEST(GameObject, rotationChangedByParent)
{
    GameEngine::geGame game;
    GameEngine::geGameObjectRef parent = game.createObject("parent");
    GameEngine::geGameObjectRef child = game.createObject("child");
    const GameEngine::Vec2D &parentRot = GameEngine::Vec2D(5,5);
    const GameEngine::Vec2D &childRot = GameEngine::Vec2D(2,2);
    parent->rotation(parentRot);
    child->rotation(childRot);

    ASSERT_EQ(parent->rotation(), parentRot);
    ASSERT_EQ(child->rotation(), childRot);

    child->parent(parent);
    ASSERT_EQ(parent->rotation(), parentRot);
    ASSERT_EQ(child->rotation(), parentRot + childRot);

    child->parent(nullptr);
    ASSERT_EQ(parent->rotation(), parentRot);
    ASSERT_EQ(child->rotation(), childRot);
}


TEST(GameObject, scaleChangedByParent)
{
    GameEngine::geGame game;
    GameEngine::geGameObjectRef parent = game.createObject("parent");
    GameEngine::geGameObjectRef child = game.createObject("child");
    const GameEngine::Vec2D &parentScale = GameEngine::Vec2D(5,5);
    const GameEngine::Vec2D &childScale = GameEngine::Vec2D(2,2);
    parent->scale(parentScale);
    child->scale(childScale);

    ASSERT_EQ(parent->scale(), parentScale);
    ASSERT_EQ(child->scale(), childScale);

    child->parent(parent);
    ASSERT_EQ(parent->scale(), parentScale);
    ASSERT_EQ(child->scale(), parentScale * childScale);

    child->parent(nullptr);
    ASSERT_EQ(parent->scale(), parentScale);
    ASSERT_EQ(child->scale(), childScale);
}

TEST(GameObject, loadGameObject)
{
    const std::string &prototype = "ObjectLoaded";

    GameEngine::geGame game;

    GameEngine::geEnvironment environment;
    environment.addPrototype(prototype, "data/goLoadTest.yaml");

    game.configEnvironment(environment);

    GameEngine::geGameObjectRef gameObject = game.createFromPrototype(prototype);
    ASSERT_EQ(gameObject->name(), "loadedFromFile");
}


TEST(GameObject, addChildDuringInitialization)
{
    GameEngine::geGame game;
    GameEngine::geGameObjectRef go = game.createObject("test");
    auto component = std::make_shared<AddOnInitializeComponent>();
    go->addComponent(component);

    // force initialize object
    std::dynamic_pointer_cast<GameEngine::Internal::GameObject>(go)->Init();

    // We ensure a component added by other during the init process of a GameObject is correctly initialized too.
    auto initializedComponent = go->getComponent<InitializeCheckComponent>();
    ASSERT_FALSE(initializedComponent.expired());
    ASSERT_TRUE(initializedComponent.lock()->getInitialized());
}

TEST(SpriteComponent, load)
{
    const std::string &prototype = "ObjectWithSpriteLoaded";

    GameEngine::geGame game;

    GameEngine::geEnvironment environment;

    game.configEnvironment(environment);
    environment.addPrototype(prototype, "data/spriteComponentLoadTest.yaml");

    GameEngine::geGameObjectRef gameObject = game.createFromPrototype(prototype);
    const std::weak_ptr<GameEngine::SpriteComponent> &component = gameObject->getComponent<GameEngine::SpriteComponent>();
    ASSERT_EQ(gameObject->name(), "loadedFromFile");
    ASSERT_TRUE(component.lock());
    ASSERT_EQ(component.lock()->filepath(),"data/1x1white.png");
}