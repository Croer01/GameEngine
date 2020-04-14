#include "gtest/gtest.h"
#include "../private/Screen.hpp"
#include <game-engine/geGame.hpp>
#include <thread>
#include <chrono>
#include <game-engine/components/AudioComponent.hpp>
#include <game-engine/components/SpriteComponent.hpp>

using namespace GameEngine;

TEST(Game, start)
{
    geGameRef game = geGame::createInstance(geEnvironment::createInstance());

    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        game->shutdown();
    });
    EXPECT_NO_THROW(
        while(game->isRunning())
        {
            game->update();
            game->render();
        }
    );
    t.join();
}


TEST(Game, renderSprite)
{
    const std::string &prototype = "ObjectTest";

    geEnvironmentRef environment = geEnvironment::createInstance();

    environment->addScene("testScene","data/testScene.yaml");
    environment->firstScene("testScene");

    environment->addPrototype(prototype, "data/spriteComponentLoadTest.yaml");

    geGameRef game = geGame::createInstance(environment);

    std::thread t([&](){
        auto spriteComponent = game->findObjectByNameInCurrentScene(prototype)->getComponent<SpriteComponent>();

        EXPECT_TRUE(spriteComponent.lock());
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        game->shutdown();
    });
    EXPECT_NO_THROW(
        while(game->isRunning())
        {
            game->update();
            game->render();
        }
    );
    t.join();
}

TEST(Game, playSound)
{
    const std::string &prototype = "ObjectTest";

    geEnvironmentRef environment = geEnvironment::createInstance();
    environment->addScene("testScene","data/testScene.yaml");
    environment->firstScene("testScene");
    environment->addPrototype(prototype, "data/audioComponentLoadTest.yaml");

    geGameRef game = geGame::createInstance(environment);

    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        auto audioComponent = game->findObjectByNameInCurrentScene(prototype)->getComponent<AudioComponent>();

        if(auto audioComponentShared = audioComponent.lock()){
            while(audioComponentShared->isPlaying());
        }
        game->shutdown();
    });

    EXPECT_NO_THROW(
        while(game->isRunning())
        {
            game->update();
            game->render();
        }
    );
    t.join();
}

TEST(Game, titleChange)
{
    geGameRef game = geGame::createInstance(geEnvironment::createInstance());
    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        const std::string &originalTitle = game->screen()->title();
        const std::string &newTitle = "This is a awesome title";
        auto screen = dynamic_cast<Internal::Screen*>(game->screen());
        screen->title(newTitle);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        ASSERT_EQ(SDL_GetWindowTitle(screen->sdlWindow()), newTitle);
        game->shutdown();
    });
    EXPECT_NO_THROW(
        while(game->isRunning())
        {
            game->update();
            game->render();
        }
    );
    t.join();
}