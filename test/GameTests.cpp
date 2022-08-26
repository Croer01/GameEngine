#include "gtest/gtest.h"
#include <game-engine/internal/Screen.hpp>
#include <game-engine/Game.hpp>
#include <thread>
#include <chrono>
#include <game-engine/components/AudioComponent.hpp>
#include <game-engine/components/SpriteComponent.hpp>

using namespace GameEngine;

TEST(Game, start)
{
    GameRef game = Game::createInstance(geEnvironment::createInstance());

    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        game->shutdown();
    });
    EXPECT_NO_THROW(
        game->init();
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

    environment->addScene("testScene","testScene.scene");
    environment->firstScene("testScene");

    environment->addPrototype(prototype, "spriteComponentLoadTest.prototype");

    GameRef game = Game::createInstance(environment);

    EXPECT_NO_THROW(
    game->init();
    );

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
    environment->addScene("testScene","testScene.scene");
    environment->firstScene("testScene");
    environment->addPrototype(prototype, "audioComponentLoadTest.prototype");

    GameRef game = Game::createInstance(environment);

    EXPECT_NO_THROW(
        game->init();
    );

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
    GameRef game = Game::createInstance(geEnvironment::createInstance());
    std::thread t([&](){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        const std::string &originalTitle = game->screen()->title();
        const std::string &newTitle = "This is a awesome title";
        auto screen = dynamic_cast<Internal::Screen*>(game->screen());
        screen->title(newTitle);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        ASSERT_EQ(screen->title(), newTitle);
        game->shutdown();
    });
    EXPECT_NO_THROW(
        game->init();
        while(game->isRunning())
        {
            game->update();
            game->render();
        }
    );
    t.join();
}

TEST(Game, writeGlobalData)
{
    GameRef game = Game::createInstance(geEnvironment::createInstance());
    
    GlobalData *data = game->getGlobalData();

    EXPECT_EQ(data->hasValue("float"), false);
    
    data->setFloat("float",2.5f);
    EXPECT_EQ(data->hasValue("float"), true);
    EXPECT_EQ(data->getFloat("float"), 2.5f);
    
    data->setFloat("float",3.5f);
    EXPECT_EQ(data->hasValue("float"), true);
    EXPECT_EQ(data->getFloat("float"), 3.5f);

    EXPECT_ANY_THROW(
        data->getString("notExist");
    );
}