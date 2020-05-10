//
// Created by adria on 10/05/2020.
//

#ifndef GAMEENGINE_RANGE_HPP
#define GAMEENGINE_RANGE_HPP

#include <game-engine/api.hpp>

namespace GameEngine
{

class PUBLICAPI Range
{
    float start_;
    float end_;
public:
    Range(float start, float end);

    float normalizedToValue(float normalized) const;
    float valueToNormalized(float value) const;
};

class PUBLICAPI ColorRange
{
    geColor start_;
    geColor end_;
public:
    ColorRange(geColor start, geColor end);

    geColor normalizedToValue(float normalized) const;
};

}

#endif //GAMEENGINE_RANGE_HPP
