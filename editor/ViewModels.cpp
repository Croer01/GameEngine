//
// Created by adria on 11/10/2019.
//

#include "ViewModels.hpp"

Vector2DData::Vector2DData() :
    xy({0.f,0.f})
{
}

ObjectData::ObjectData()
{
    scale_.xy = {1.f, 1.f};
    rotation_ = 0;
}

PrototypeReference::PrototypeReference()
{
    scale_.xy = {1.f, 1.f};
    rotation_ = 0;
}

ColorData::ColorData() :
    rgb({1.f, 1.f, 1.f})
{}
