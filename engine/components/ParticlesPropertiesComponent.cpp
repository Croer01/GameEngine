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

}

PropertySetBase *ParticlesPropertiesComponent::getProperties() const
{
    auto properties = new PropertySet<ParticlesPropertiesComponent>();

    properties->add(new PropertyFilePath<ParticlesPropertiesComponent>(
        "imageSource",
        &ParticlesPropertiesComponent::imageSource,
        &ParticlesPropertiesComponent::imageSource,
        "",
        FileType::IMAGE,
        true
    ));

    // Rotation is in degrees
    properties->add(new Property<ParticlesPropertiesComponent,float>(
        "minRotation",
        &ParticlesPropertiesComponent::minRotation,
        &ParticlesPropertiesComponent::minRotation,
        0
        ));

    properties->add(new Property<ParticlesPropertiesComponent,float>(
        "maxRotation",
        &ParticlesPropertiesComponent::maxRotation,
        &ParticlesPropertiesComponent::maxRotation,
        0
    ));

    properties->add(new Property<ParticlesPropertiesComponent,float>(
        "startSpeed",
        &ParticlesPropertiesComponent::startSpeed,
        &ParticlesPropertiesComponent::startSpeed,
        10
    ));

    properties->add(new Property<ParticlesPropertiesComponent,float>(
        "endSpeed",
        &ParticlesPropertiesComponent::endSpeed,
        &ParticlesPropertiesComponent::endSpeed,
        10
    ));

    properties->add(new Property<ParticlesPropertiesComponent,float>(
        "startScale",
        &ParticlesPropertiesComponent::startScale,
        &ParticlesPropertiesComponent::startScale,
        1
    ));

    properties->add(new Property<ParticlesPropertiesComponent,float>(
        "endScale",
        &ParticlesPropertiesComponent::endScale,
        &ParticlesPropertiesComponent::endScale,
        1
    ));

    properties->add(new Property<ParticlesPropertiesComponent,float>(
        "startOpacity",
        &ParticlesPropertiesComponent::startOpacity,
        &ParticlesPropertiesComponent::startOpacity,
        1
    ));

    properties->add(new Property<ParticlesPropertiesComponent,float>(
        "endOpacity",
        &ParticlesPropertiesComponent::endOpacity,
        &ParticlesPropertiesComponent::endOpacity,
        1
    ));

    properties->add(new Property<ParticlesPropertiesComponent,float>(
        "startLifetime",
        &ParticlesPropertiesComponent::startLifetime,
        &ParticlesPropertiesComponent::startLifetime,
        1
    ));

    properties->add(new Property<ParticlesPropertiesComponent,float>(
        "endLifetime",
        &ParticlesPropertiesComponent::endLifetime,
        &ParticlesPropertiesComponent::endLifetime,
        1
    ));

    properties->add(new Property<ParticlesPropertiesComponent, geColor>(
        "startTint",
        &ParticlesPropertiesComponent::startTint,
        &ParticlesPropertiesComponent::startTint,
        1
    ));

    properties->add(new Property<ParticlesPropertiesComponent, geColor>(
        "endTint",
        &ParticlesPropertiesComponent::endTint,
        &ParticlesPropertiesComponent::endTint,
        1
    ));

    return properties;
}

void ParticlesPropertiesComponent::preInit()
{
    graphicsEngine_ = gameObject()->game()->graphicsEngine();
    particleSprite_ = std::make_shared<Internal::GraphicSprite>(imageSource_);
}

Internal::ParticleRef ParticlesPropertiesComponent::createParticle()
{
    auto particle = std::make_shared<Internal::Particle>(graphicsEngine_, particleSprite_);
    particle->setRotation(rotation_.normalizedToValue(random_.getFloat()));
    particle->setSpeed(startSpeed_, endSpeed_);
    particle->setScale(startScale_, endScale_);
    particle->setOpacity(startOpacity_, endOpacity_);
    particle->setTimeLife(lifetime_.normalizedToValue(random_.getFloat()));
    particle->setColor(startTint_, endTint_);

    return particle;
}

std::string ParticlesPropertiesComponent::imageSource() const
{
    return imageSource_;
}

void ParticlesPropertiesComponent::imageSource(const std::string &value)
{
    imageSource_ = value;

    if(imageSource_.empty())
        particleSprite_.reset();
    else if (graphicsEngine_ != nullptr)
        particleSprite_ = std::make_shared<Internal::GraphicSprite>(imageSource_);
}

void ParticlesPropertiesComponent::minRotation(const float &value)
{
   rotation_ = Range(value * (M_PI/180.f), rotation_.getEnd());
}

float ParticlesPropertiesComponent::minRotation() const
{
   return (rotation_.getStart() * 180) / M_PI;
}

void ParticlesPropertiesComponent::maxRotation(const float &value)
{
    rotation_ = Range(rotation_.getStart(), value * (M_PI/180.f));
}

float ParticlesPropertiesComponent::maxRotation() const
{
    return (rotation_.getEnd() * 180) / M_PI;
}

float ParticlesPropertiesComponent::startSpeed() const
{
    return startSpeed_;
}

void ParticlesPropertiesComponent::startSpeed(const float &value)
{
    startSpeed_ = value;
}

float ParticlesPropertiesComponent::endSpeed() const
{
    return endSpeed_;
}

void ParticlesPropertiesComponent::endSpeed(const float &value)
{
    endSpeed_ = value;
}

float ParticlesPropertiesComponent::startScale() const
{
    return startScale_;
}

void ParticlesPropertiesComponent::startScale(const float &value)
{
    startScale_ = value;
}

float ParticlesPropertiesComponent::endScale() const
{
    return endScale_;
}

void ParticlesPropertiesComponent::endScale(const float &value)
{
    endScale_ = value;
}

float ParticlesPropertiesComponent::startOpacity() const
{
    return startOpacity_;
}

void ParticlesPropertiesComponent::startOpacity(const float &value)
{
    startOpacity_ = value;
}

float ParticlesPropertiesComponent::endOpacity() const
{
    return endOpacity_;
}

void ParticlesPropertiesComponent::endOpacity(const float &value)
{
    endOpacity_ = value;
}

float ParticlesPropertiesComponent::startLifetime() const
{
    return lifetime_.getStart();
}

void ParticlesPropertiesComponent::startLifetime(const float &value)
{
    lifetime_ = Range(value, lifetime_.getEnd());
}

float ParticlesPropertiesComponent::endLifetime() const
{
    return lifetime_.getEnd();
}

void ParticlesPropertiesComponent::endLifetime(const float &value)
{
    lifetime_ = Range(lifetime_.getStart(), value);
}

geColor ParticlesPropertiesComponent::startTint() const
{
    return startTint_;
}

void ParticlesPropertiesComponent::startTint(const geColor &value)
{
    startTint_ = value;
}

geColor ParticlesPropertiesComponent::endTint() const
{
    return endTint_;
}

void ParticlesPropertiesComponent::endTint(const geColor &value)
{
    endTint_ = value;
}

}
