#include "gtest/gtest.h"
#include <game-engine/geGame.hpp>
#include <game-engine/geGameObject.hpp>
#include <thread>
#include <chrono>

TEST(Game, start)
{
    GameEngine::geGame game;

    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        game.shutdown();
    });
    EXPECT_NO_THROW(
        EXPECT_HRESULT_SUCCEEDED(game.loop());
    );
    t.join();
}