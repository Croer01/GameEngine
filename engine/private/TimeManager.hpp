//
// Created by adria on 18/04/2020.
//

#ifndef GAMEENGINE_TIMEMANAGER_HPP
#define GAMEENGINE_TIMEMANAGER_HPP


#include <memory>
#include "GameObject.hpp"

namespace GameEngine {

class UITextComponent;

namespace Internal {

class TimeManager
{
    std::unique_ptr<GameObject> goFpsCounter_;
    std::shared_ptr<UITextComponent> fpsCounter_;
    float fixedFrameRateMs_; // in millis
    bool frameSkipped_;
    float fpsAcumulator_;
    float fpsFixedAcumulator_;
    float accumulateElapsed_;
    float elapsedTime_;

    unsigned int lastTime_;

    void createFpsCounter(Game *game);
public:
    explicit TimeManager(Game *game);
    ~TimeManager();

    void calculateTime();
    float getElapsedTime() const;
    float getPhysicsElapsedTime() const;
    /** If the frame rate is configured to be fixed, this won't return true until the accumulated elapsed time is
     *  equal or greather than the expected frame rate.
     */
    bool isFrameSkipped() const;
};

}
}

#endif //GAMEENGINE_TIMEMANAGER_HPP
