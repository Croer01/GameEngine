//
// Created by adria on 01/01/2019.
//

#ifndef SPACEINVADERS_RANDOM_HPP
#define SPACEINVADERS_RANDOM_HPP


#include <random>

class Random {
    std::mt19937 generator_;
    std::uniform_real_distribution<float> distributionReal_;
public:
    Random();
    float getFloat();
    int getRange(int min, int max);
    float getRange(float min, float max);
};


#endif //SPACEINVADERS_RANDOM_HPP
