//
// Created by adria on 01/01/2019.
//

#include <ctime>
#include <string>
#include <game-engine/Random.hpp>
#include <stdexcept>

namespace GameEngine {
    Random::Random() : generator_(std::random_device()()),
                       distributionReal_(std::uniform_real_distribution<float>(0.f, 1.f)) {}

    float Random::getFloat() {
        return distributionReal_(generator_);
    }

    int Random::getRange(int min, int max) {
        if (min < 0)
            throw std::invalid_argument("min must be greater than 0");

        if (max < min)
            throw std::invalid_argument(
                    "max(" + std::to_string(max) + ") must be greater than min(" + std::to_string(min) + ")");

        std::uniform_int_distribution<int> randomDist(0, max - min);

        return min + randomDist(generator_);
    }

    float Random::getRange(float min, float max) {
        if (min < 0)
            throw std::invalid_argument("min must be greater than 0");

        if (max < min)
            throw std::invalid_argument(
                    "max(" + std::to_string(max) + ") must be greater than min(" + std::to_string(min) + ")");

        std::uniform_real_distribution<float> randomDist(0, max - min);

        return min + randomDist(generator_);
    }
}