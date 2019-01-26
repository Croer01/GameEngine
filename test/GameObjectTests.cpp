#include "gtest/gtest.h"
#include <game-engine/geGame.hpp>
#include <game-engine/geGameObject.hpp>

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