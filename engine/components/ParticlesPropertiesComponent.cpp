//
// Created by adria on 08/05/2020.
//

#include <game-engine/components/ParticlesPropertiesComponent.hpp>

#include <memory>
#include <game-engine/Game.hpp>

# define M_PI 3.14159265358979323846f  /* pi */

namespace GameEngine
{

ParticlesPropertiesComponent::ParticlesPropertiesComponent() :
    lifetime_(1.f,1.f), rotation_(0.f, 0.f), graphicsEngine_(nullptr)
{
    setFilePathPropertyObserver("imageSource", [this](){ updateSprite(); });
    setPropertyObserver<float>("startLifetime", [this](){
        lifetime_ = Range(getPropertyValue<float>("startLifetime"), lifetime_.getEnd());
    });
    setPropertyObserver<float>("endLifetime", [this](){
        lifetime_ = Range(lifetime_.getStart(), getPropertyValue<float>("endLifetime"));
    });
    setPropertyObserver<float>("minRotation", [this](){
        rotation_ = Range(getPropertyValue<float>("minRotation") * (M_PI/180.f), rotation_.getEnd());
    });
    setPropertyObserver<float>("maxRotation", [this](){
        rotation_ = Range(rotation_.getStart(), getPropertyValue<float>("maxRotation") * (M_PI/180.f));
    });
}

void ParticlesPropertiesComponent::preInit()
{
    graphicsEngine_ = gameObject()->game()->graphicsEngine();
    updateSprite();
}

Internal::ParticleRef ParticlesPropertiesComponent::createParticle()
{
    auto particle = std::make_shared<Internal::Particle>(graphicsEngine_, particleSprite_);
    particle->setRotation(rotation_.normalizedToValue(random_.getFloat()));
    particle->setSpeed(getPropertyValue<float>("startSpeed"), getPropertyValue<float>("endSpeed"));
    particle->setScale(getPropertyValue<float>("startScale"), getPropertyValue<float>("endScale"));
    particle->setOpacity(getPropertyValue<float>("startOpacity"), getPropertyValue<float>("endOpacity"));
    particle->setTimeLife(lifetime_.normalizedToValue(random_.getFloat()));
    particle->setColor(getPropertyValue<geColor>("startTint"), getPropertyValue<geColor>("endTint"));

    return particle;
}

void ParticlesPropertiesComponent::updateSprite()
{
    particleSprite_.reset();
    std::string imageSource = getFilePathPropertyValue("imageSource");
    if (!imageSource.empty() && graphicsEngine_ != nullptr)
        particleSprite_ = std::make_shared<Internal::GraphicSprite>(imageSource);
}

}
