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

Vector2DData PrototypeReference::getPosition() const
{
    return position_;
}

void PrototypeReference::setPosition(const Vector2DData &position)
{
    position_ = position;
    notify(PrototypeReferenceEvent::PositionChanged);
}

Vector2DData PrototypeReference::getScale() const
{
    return scale_;
}

void PrototypeReference::setScale(const Vector2DData &scale)
{
    scale_ = scale;
    notify(PrototypeReferenceEvent::ScaleChanged);
}

float PrototypeReference::getRotation() const
{
    return rotation_;
}

void PrototypeReference::setRotation(float rotation)
{
    rotation_ = rotation;
    notify(PrototypeReferenceEvent::RotationChanged);
}

ColorData::ColorData() :
    rgb({1.f, 1.f, 1.f})
{}

void SceneData::addObject(const PrototypeReferenceRef &object)
{
    objects_.push_back(object);
    PrototypeReferenceRef *ref = &const_cast<PrototypeReferenceRef&>(object);
    notify(SceneDataEvent::ObjectAdded, ref);
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

void SceneData::selectObject(const PrototypeReferenceRef &object)
{
    notify(SceneDataEvent::ObjectSelected, object.get());
    selected_ = object;
}

PrototypeReferenceRef SceneData::getSelectedObject() const
{
    return selected_;
}
