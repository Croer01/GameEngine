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

void SceneData::addObject(const PrototypeReferenceRef &object)
{
    objects_.push_back(object);
    notify(SceneDataEvent::ObjectAdded, object.get());
}

size_t SceneData::objectsSize() const
{
    return objects_.size();
}

PrototypeReferenceRef SceneData::getObject(int index) const
{
    assert(0 <= index && index <= objects_.size() - 1);
    return objects_[index];
}

void SceneData::deleteObject(int index)
{
    auto object = *(objects_.begin() + index);
    objects_.erase(objects_.begin() + index);
    notify(SceneDataEvent::ObjectDeleted, object.get());
}

SceneData::const_iterator SceneData::getObjectsBegin() const
{
    return objects_.begin();
}

SceneData::const_iterator SceneData::getObjectsEnd() const
{
    return objects_.end();
}
