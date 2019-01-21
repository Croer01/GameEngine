//
// Created by adria on 28/10/2018.
//

#include <cmath>
#include <yaml-cpp/yaml.h>
#include "Screen.hpp"
namespace GameEngine {
namespace Internal {
    Screen::Screen(const std::string &filePath) {
        YAML::Node screenConfig = YAML::LoadFile(filePath);

        title_ = screenConfig["title"].as<std::string>();

        YAML::Node windowSize = screenConfig["windowSize"];
        YAML::Node virtualSize = screenConfig["virtualSize"];

        deviceWidth_ = windowSize[0].as<int>();
        deviceHeight_ = windowSize[1].as<int>();
        virtualWidth_ = virtualSize[0].as<int>();
        virtualHeight_ = virtualSize[1].as<int>();

        //Calculate the real size of the virtual screen when it will render
        float targetAspectRatio = (float) virtualWidth_ / virtualHeight_;

        // figure out the largest area that fits in this resolution at the desired aspect ratio
        calculatedWidth_ = deviceWidth_;
        calculatedHeight_ = std::lround(calculatedWidth_ / targetAspectRatio + 0.5f);

        if (calculatedHeight_ > deviceHeight_) {
            //It doesn't fit our height, we must switch to pillarbox then
            calculatedHeight_ = deviceHeight_;
            calculatedWidth_ = std::lround(calculatedHeight_ * targetAspectRatio + 0.5f);
        }

        // set up the new viewport centered in the backbuffer
        calculatedX_ = (deviceWidth_ / 2) - (calculatedWidth_ / 2);
        calculatedY_ = (deviceHeight_ / 2) - (calculatedHeight_ / 2);

        YAML::Node backgroundColor = screenConfig["backgroundColor"];
        background_ = glm::vec3(backgroundColor[0].as<float>(), backgroundColor[1].as<float>(),
                                backgroundColor[2].as<float>());

        pixelPerfect_ = screenConfig["pixelPerfect"].as<bool>();
    }

    std::string Screen::getTitle() const {
        return title_;
    }

    int Screen::getWindowWidth() const {
        return deviceWidth_;
    }

    int Screen::getWindowHeight() const {
        return deviceHeight_;
    }

    int Screen::getCalculatedX() const {
        return calculatedX_;
    }

    int Screen::getCalculatedY() const {
        return calculatedY_;
    }

    int Screen::getCalculatedWidth() const {
        return calculatedWidth_;
    }

    int Screen::getCalculatedHeight() const {
        return calculatedHeight_;
    }

    glm::vec3 Screen::getBackgroundColor() const {
        return background_;
    }

    int Screen::getVirtualWidth() const {
        return virtualWidth_;
    }

    int Screen::getVirtualHeight() const {
        return virtualHeight_;
    }

    bool Screen::isPixelPerfect() const {
        return pixelPerfect_;
    }
}
}