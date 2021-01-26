//
// Created by adria on 10/05/2020.
//

#include <game-engine/internal/graphics/Particle.hpp>
#include <game-engine/internal/graphics/GraphicsEngine.hpp>

namespace GameEngine{
namespace Internal{

Particle::Particle(GraphicsEngine *graphicEngine, const std::shared_ptr<Internal::Graphic> &graphic) :
    size_(1.f),
    active_(false),
    timeLife_(0.f),
    speed_(Range(0.f,1.f)),
    scale_(Range(1.f,1.f)),
    opacity_(Range(1.f,1.f)),
    color_(ColorRange({0.f}, {0.f})),
    lifetimeAccumulator_(0.f),
    graphicEngine_(graphicEngine),
    direction_(Vec2D(0.f, -1.f))
{
    graphic_ = std::make_shared<Internal::GraphicHolder>(graphic);
    graphicEngine_->registerGraphic(graphic_);
    // set some mandatory configuration
    graphic_->setAnchor(Internal::GraphicAnchor::MIDDLE_CENTER);
    graphic_->setActive(active_);
    graphic_->setModelTransform(position_, std::atan2(direction_.x, direction_.y),Vec2D(1.f,1.f));
}

Particle::~Particle()
{
    if(graphicEngine_ != nullptr)
        graphicEngine_->unregisterGraphic(graphic_);
}

void Particle::setPosition(const Vec2D &position)
{
    position_ = position;
}

void Particle::setRotation(float rotation)
{
    // precondition: angle in radians
    direction_.x = std::cos(rotation);
    direction_.y = std::sin(rotation);
}

void Particle::setSize(float size)
{
    size_ = size;
}

bool Particle::isActive() const
{
    return active_;
}

void Particle::setActive(bool active)
{
    active_ = active;
    if(!active_)
        lifetimeAccumulator_ = 0;
    graphic_->setActive(active_);
}

void Particle::setTimeLife(float timeLife)
{
    timeLife_ = timeLife;
}

void Particle::setSpeed(float start, float end)
{
    speed_ = Range(start, end);
}

void Particle::setScale(float start, float end)
{
    scale_ = Range(start, end);
}

void Particle::setColor(geColor start, geColor end)
{
    color_ = ColorRange(start, end);
}

void Particle::update(float elapsedTime)
{
    lifetimeAccumulator_ += elapsedTime;
    // use setActive to update also the graphics
    setActive(lifetimeAccumulator_ < timeLife_);

    if(active_)
    {
        float t = lifetimeAccumulator_ / timeLife_;

        float speed = speed_.normalizedToValue(t) * elapsedTime;
        float scale = scale_.normalizedToValue(t);
        position_.x += direction_.x * speed;
        position_.y += direction_.y * speed;
        graphic_->setModelTransform(position_, std::atan2(direction_.x, direction_.y),Vec2D(scale, scale));

        geColor tint = color_.normalizedToValue(t);
        float opacity = opacity_.normalizedToValue(t);
        graphic_->setTintColor(tint, opacity);
    }
}

void Particle::setOpacity(float start, float end)
{
    opacity_ = Range(start, end);
}

}
}
