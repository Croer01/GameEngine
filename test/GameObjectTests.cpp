#include "gtest/gtest.h"
#include <game-engine/internal/GameObject.hpp>
#include <game-engine/Game.hpp>
#include <game-engine/geGameObject.hpp>
#include <game-engine/components/SpriteComponent.hpp>
#include <ostream>

using namespace GameEngine;
namespace GameEngine{
    std::ostream& operator<<(std::ostream& os, const Vec2D& v) {
      os.precision(5);
      return os << "x = " << v.x << ", y = " << v.y;
    }
}

class InitializeCheckComponent : public geComponent
{
    COMPONENT_BOILERPLATE(InitializeCheckComponent, ComponentData)
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

class AddOnInitializeComponent : public geComponent
{
    COMPONENT_BOILERPLATE(AddOnInitializeComponent, ComponentData)

    void init() override
    {
        auto component = std::make_shared<InitializeCheckComponent>();
        gameObject()->addComponent(component);
    }
};

TEST(GameObject, createObject)
{
    GameRef game = Game::createInstance(geEnvironment::createInstance());

    geGameObjectRef go = game->createObject("test1");

    ASSERT_EQ(go->name(), "test1");
}

TEST(GameObject, changeName)
{
    const std::string &oldName = "old name";
    const std::string &newName = "new name";
    GameRef game = Game::createInstance(geEnvironment::createInstance());
    geGameObjectRef go = game->createObject(oldName);

    ASSERT_EQ(go->name(), oldName);
    go->name(newName);
    ASSERT_EQ(go->name(), newName);
}


TEST(GameObject, positionChangedByParent)
{
    GameRef game = Game::createInstance(geEnvironment::createInstance());
    geGameObjectRef parent = game->createObject("parent");
    geGameObjectRef child = game->createObject("child");
    const Vec2D &parentPos = Vec2D(5,5);
    const Vec2D &childPos = Vec2D(2,2);
    parent->position(parentPos);
    child->position(childPos);

    ASSERT_EQ(parent->position(), parentPos);
    ASSERT_EQ(child->position(), childPos);

    child->parent(parent.get());
    ASSERT_EQ(parent->position(), parentPos);
    ASSERT_EQ(child->position(), parentPos + childPos);

    child->parent(nullptr);
    ASSERT_EQ(parent->position(), parentPos);
    ASSERT_EQ(child->position(), childPos);

}

TEST(GameObject, rotationChangedByParent)
{
    GameRef game = Game::createInstance(geEnvironment::createInstance());
    geGameObjectRef parent = game->createObject("parent");
    geGameObjectRef child = game->createObject("child");
    float parentRot = 5;
    float childRot = 2;
    parent->rotation(parentRot);
    child->rotation(childRot);

    ASSERT_EQ(parent->rotation(), parentRot);
    ASSERT_EQ(child->rotation(), childRot);

    child->parent(parent.get());
    ASSERT_EQ(parent->rotation(), parentRot);
    ASSERT_EQ(child->rotation(), parentRot + childRot);

    child->parent(nullptr);
    ASSERT_EQ(parent->rotation(), parentRot);
    ASSERT_EQ(child->rotation(), childRot);
}


TEST(GameObject, scaleChangedByParent)
{
    GameRef game = Game::createInstance(geEnvironment::createInstance());
    geGameObjectRef parent = game->createObject("parent");
    geGameObjectRef child = game->createObject("child");
    const Vec2D &parentScale = Vec2D(5,5);
    const Vec2D &childScale = Vec2D(2,2);
    parent->scale(parentScale);
    child->scale(childScale);

    ASSERT_EQ(parent->scale(), parentScale);
    ASSERT_EQ(child->scale(), childScale);

    child->parent(parent.get());
    ASSERT_EQ(parent->scale(), parentScale);
    ASSERT_EQ(child->scale(), parentScale * childScale);

    child->parent(nullptr);
    ASSERT_EQ(parent->scale(), parentScale);
    ASSERT_EQ(child->scale(), childScale);
}

TEST(GameObject, loadGameObject)
{
    const std::string &prototype = "ObjectLoaded";

    geEnvironmentRef environment = geEnvironment::createInstance();
    environment->addPrototype(prototype, "goLoadTest.prototype");

    GameRef game = Game::createInstance(environment);

    geGameObjectRef gameObject = game->createFromPrototype(prototype);
    ASSERT_EQ(gameObject->name(), "loadedFromFile");
}


TEST(GameObject, addChildDuringInitialization)
{
    GameRef game = Game::createInstance(geEnvironment::createInstance());
    geGameObjectRef go = game->createObject("test");
    auto component = std::make_shared<AddOnInitializeComponent>();
    go->addComponent(component);

    // force initialize object
    std::dynamic_pointer_cast<Internal::GameObject>(go)->preInit();
    std::dynamic_pointer_cast<Internal::GameObject>(go)->Init();

    // We ensure a component added by other during the init process of a GameObject is correctly initialized too.
    auto initializedComponent = go->getComponent<InitializeCheckComponent>();
    ASSERT_FALSE(initializedComponent.expired());
    ASSERT_TRUE(initializedComponent.lock()->getInitialized());
}

TEST(SpriteComponent, load)
{
    const std::string &prototype = "ObjectWithSpriteLoaded";

    geEnvironmentRef environment = geEnvironment::createInstance();
    environment->addPrototype(prototype, "spriteComponentLoadTest.prototype");

    GameRef game = Game::createInstance(environment);

    geGameObjectRef gameObject = game->createFromPrototype(prototype);
    const std::weak_ptr<SpriteComponent> &component = gameObject->getComponent<SpriteComponent>();
    ASSERT_EQ(gameObject->name(), "loadedFromFile");
    ASSERT_TRUE(component.lock());
    ASSERT_EQ(component.lock()->getFilePathPropertyValue("filePath"),"data/1x1white.png");
}

TEST(GameObject, transformPositionToLocal)
{
    GameRef game = Game::createInstance(geEnvironment::createInstance());
    geGameObjectRef go = game->createObject("parent");
    const Vec2D goPos = Vec2D(5, 5);
    const Vec2D externalPos = Vec2D(2, 2);
    go->position(goPos);

    ASSERT_EQ(go->position(), goPos);
    ASSERT_EQ( go->transformToLocalPosition(externalPos), Vec2D(-3,-3) );

    go->rotation(90.f * /*one radian*/0.0174532925);
    ASSERT_EQ(go->position(), goPos);
    auto localPos = go->transformToLocalPosition(externalPos);
    auto expectedPos = Vec2D(-3, 3);
    ASSERT_FLOAT_EQ(localPos.x, expectedPos.x);
    ASSERT_FLOAT_EQ(localPos.y, expectedPos.y);

    go->rotation(-90.f * /*one radian*/0.0174532925);
    ASSERT_EQ(go->position(), goPos);
    localPos = go->transformToLocalPosition(externalPos);
    expectedPos = Vec2D(3, -3);
    ASSERT_FLOAT_EQ(localPos.x, expectedPos.x);
    ASSERT_FLOAT_EQ(localPos.y, expectedPos.y);
}

TEST(GameObject, transformPositionToWorld)
{
    GameRef game = Game::createInstance(geEnvironment::createInstance());
    geGameObjectRef go = game->createObject("parent");
    const Vec2D goPos = Vec2D(5, 5);
    const Vec2D localPos = Vec2D(2, 2);
    go->position(goPos);

    ASSERT_EQ(go->position(), goPos);
    ASSERT_EQ( go->transformToWorldPosition(localPos), Vec2D(7, 7) );

    go->rotation(90.f * /*one radian*/0.0174532925);
    ASSERT_EQ(go->position(), goPos);
    auto externalPos = go->transformToWorldPosition(localPos);
    auto expectedPos = Vec2D(3, 7);
    ASSERT_FLOAT_EQ(externalPos.x, expectedPos.x);
    ASSERT_FLOAT_EQ(externalPos.y, expectedPos.y);

    go->rotation(-90.f * /*one radian*/0.0174532925);
    ASSERT_EQ(go->position(), goPos);
    externalPos = go->transformToWorldPosition(localPos);
    expectedPos = Vec2D(7, 3);
    ASSERT_FLOAT_EQ(externalPos.x, expectedPos.x);
    ASSERT_FLOAT_EQ(externalPos.y, expectedPos.y);
}