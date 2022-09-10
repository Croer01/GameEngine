//
// Created by adria on 10/05/2020.
//

#include <cassert>
#include <game-engine/api.hpp>
#include <game-engine/Range.hpp>

namespace GameEngine
{

Range::Range(float start, float end) : start_(start), end_(end)
{}

float Range::normalizedToValue(float normalized) const
{
    assert(0 <= normalized && normalized <= 1);
    return start_ + (end_ - start_) * normalized;
}

float Range::valueToNormalized(float value) const
{
    assert(std::fmin(start_, end_) <= value && value <= std::fmax(start_, end_));
    return (value - start_) / (end_ - start_);
}

float Range::getStart() const
{
    return start_;
}

float Range::getEnd() const
{
    return end_;
}

ColorRange::ColorRange(geColor start, geColor end) : start_(start), end_(end)
{}

geColor ColorRange::normalizedToValue(float normalized) const
{
    float r = start_.r + (end_.r - start_.r) * normalized;
    float g = start_.g + (end_.g - start_.g) * normalized;
    float b = start_.b + (end_.b - start_.b) * normalized;
    return geColor(r, g, b);
}

const geColor &ColorRange::getStart() const
{
    return start_;
}

const geColor &ColorRange::getEnd() const
{
    return end_;
}
}
