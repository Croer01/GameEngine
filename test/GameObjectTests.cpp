//
// Created by adria on 20/01/2019.
//

#include "gtest/gtest.h"
#include <game-engine/geGame.hpp>
#include <game-engine/geGameObject.hpp>

TEST(GameObjectEvents, transformChange)
{
    GameEngine::geGame game;

    GameEngine::geGameObjectRef go = game.createObject("test1");

    ASSERT_EQ(go->name(), "test1");
}