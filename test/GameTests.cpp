#include "gtest/gtest.h"
#include <game-engine/geGame.hpp>
#include <game-engine/geGameObject.hpp>
#include <thread>
#include <chrono>
#include <game-engine/components/AudioComponent.hpp>

TEST(Game, start)
{
    GameEngine::geGame game;

    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        game.shutdown();
    });
    EXPECT_NO_THROW(
        EXPECT_HRESULT_SUCCEEDED(game.loop());
    );
    t.join();
}


TEST(Game, renderSprite)
{
    const std::string &prototype = "ObjectTest";

    GameEngine::geGame game;

    GameEngine::geEnvironment environment;

    environment.addScene("testScene","data/testScene.yaml");
    environment.firstScene("testScene");

    game.configEnvironment(environment);
    game.init();
    //TODO: fix how to register prototypes (maybe can add a flag into graphics to know when the gl context is created
    environment.addPrototype(prototype, "data/spriteComponentLoadTest.yaml");

    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        game.shutdown();
    });
    EXPECT_NO_THROW(
            EXPECT_HRESULT_SUCCEEDED(game.loop());
    );
    t.join();
}

TEST(Game, playSound)
{
    const std::string &prototype = "ObjectTest";

    GameEngine::geGame game;

    GameEngine::geEnvironment environment;

    environment.addScene("testScene","data/testScene.yaml");
    environment.firstScene("testScene");

    game.configEnvironment(environment);
    game.init();
    //TODO: fix how to register prototypes (maybe can add a flag into graphics to know when the gl context is created
    environment.addPrototype(prototype, "data/audioComponentLoadTest.yaml");

    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto audioComponent = game.findObjectByNameInCurrentScene(prototype)->getComponent<GameEngine::AudioComponent>();

        if(auto audioComponentShared = audioComponent.lock()){
            while(audioComponentShared->isPlaying());
        }
        game.shutdown();
    });

    EXPECT_NO_THROW(
        EXPECT_HRESULT_SUCCEEDED(game.loop());
    );
    t.join();
}