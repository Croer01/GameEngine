//
// Created by adria on 18/04/2020.
//

#include <SDL_timer.h>
#include <game-engine/internal/TimeManager.hpp>
#include <game-engine/components/ui/UITextComponent.hpp>

namespace GameEngine {
namespace Internal {

TimeManager::TimeManager(Game *game)
{
    createFpsCounter(game);

    lastTime_ = SDL_GetTicks();
    accumulateElapsed_ = 0.f;
    elapsedTime_ = 0;
    // TODO: do this configurable
    fixedFrameRateMs_ = 1.f / 60.f;
    frameSkipped_ = false;
}

void TimeManager::createFpsCounter(Game *game)
{
    goFpsCounter_ = std::make_unique<GameObject>("FPS Counter");
    fpsCounter_ = std::make_shared<UITextComponent>();
    fpsCounter_->setData(fpsCounter_->instantiateData());
    fpsCounter_->setPropertyValue<std::string>("id", "fps");
    fpsCounter_->setPropertyValue<int>("fontSize", 10);
    fpsCounter_->setPropertyValue<bool>("visible", true);
    goFpsCounter_->addComponent(fpsCounter_);
    goFpsCounter_->game(game);
    goFpsCounter_->preInit();
    goFpsCounter_->Init();
    fpsAcumulator_ = 0.f;
}

TimeManager::~TimeManager()
{
    fpsCounter_.reset();
    goFpsCounter_.reset();
}

void TimeManager::calculateTime()
{
    unsigned int currentTime = SDL_GetTicks();
    elapsedTime_ = (currentTime - lastTime_) / 1000.f;
    accumulateElapsed_ += elapsedTime_;
    lastTime_ = currentTime;

    fpsAcumulator_ = (fpsAcumulator_ + elapsedTime_) / 2.f;

    frameSkipped_ = accumulateElapsed_ < fixedFrameRateMs_;
    if(!frameSkipped_)
    {
        accumulateElapsed_ -= fixedFrameRateMs_;
        fpsFixedAcumulator_ = (fpsFixedAcumulator_ + fixedFrameRateMs_) / 2.f;
        elapsedTime_ = fixedFrameRateMs_;
    }

    std::stringstream ss;
    ss << "fps: " << (1.0f / fpsFixedAcumulator_) << " (" << (1.0f / fpsAcumulator_) << ")";
    fpsCounter_->setPropertyValue<std::string>("text", ss.str());
//    goFpsCounter_->Update(elapsedTime_);
}
float TimeManager::getElapsedTime() const
{
    return elapsedTime_;
}

float TimeManager::getPhysicsElapsedTime() const
{
    return 1.f / 120.f;
}

bool TimeManager::isFrameSkipped() const
{
    return frameSkipped_;
}

}
}
