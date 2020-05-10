//
// Created by adria on 03/05/2020.
//

#include <game-engine/components/ParticleEmitterComponent.hpp>
#include "../../private/graphics/GraphicGeometry.hpp"
#include "../../private/Game.hpp"

namespace GameEngine
{

void ParticleEmitterComponent::preInit()
{
    maxParticles_ = 20;
    pool_.reserve(maxParticles_);

    std::vector<Vec2D> path = {
        Vec2D(0.f,0.f),
        Vec2D(10.f,0.f),
        Vec2D(10.f,10.f),
        Vec2D(0.f,10.f)
    };
    std::shared_ptr<Internal::GraphicGeometry> graphicLoaded = std::make_shared<Internal::GraphicGeometry>(path);
    Internal::GraphicsEngine *graphicsEngine = dynamic_cast<Internal::Game *>(gameObject()->game())->graphicsEngine();

    Range rotation = Range(0,2*3.1416f);
    for(auto i = 0; i < maxParticles_; i++)
    {
        const Internal::ParticleRef &particle = std::make_shared<Internal::Particle>(graphicsEngine, graphicLoaded);
        particle->setRotation(rotation.normalizedToValue(i / (float)maxParticles_));
        particle->setSpeed(10,100);
        pool_.push_back(particle);
    }

    //TODO: remove this
    emitOnInit_ = true;
    infinite_ = true;
    spawnFrequency_ = 0.05f;
    //////////////////////

    if(emitOnInit_)
        emit();
}

void ParticleEmitterComponent::Update(float elapsedTime)
{
    // first calculate and check if the emitter can continue spawning particles
    if(emitting_ && !infinite_)
    {
        timeLifeAccumulator_ += elapsedTime;
        if(timeLifeAccumulator_ >= timeLife_)
        {
            emitting_ = false;
            timeLifeAccumulator_ = 0;
        }
    }

    // check if the emitter can spawn a new particle
    if(emitting_)
    {
        spawnTimeAccumulator_ += elapsedTime;

        // maybe the emitter can spawn particles faster than the elapsed time
        while (spawnTimeAccumulator_ >= spawnFrequency_)
        {
            spawnParticle();
            spawnTimeAccumulator_ -= spawnFrequency_;
        }
    }

    // update all the active particles
    auto next = particlesActive_.begin();

    while (next != particlesActive_.end())
    {
        (*next)->update(elapsedTime);
        if(!(*next)->isActive())
            next = particlesActive_.erase(next);
        else
            next++;
    }
}

void ParticleEmitterComponent::spawnParticle()
{
    auto it = std::find_if(pool_.begin(), pool_.end(), [](const Internal::ParticleRef &p)
    {
        return !p->isActive();
    });
    
    if(it != pool_.end())
    {
        Internal::ParticleRef particle = *it;
        particle->setActive(true);
        particle->setPosition(gameObject()->position());
        // TODO: set other stuff from particle component
        particle->setTimeLife(1.f);
        particlesActive_.push_back(particle);
    }
}

void ParticleEmitterComponent::emit()
{
    emitting_ = true;
    spawnTimeAccumulator_ = 0;
    timeLifeAccumulator_ = 0;

    auto next = particlesActive_.begin();

    while (next != particlesActive_.end())
    {
        (*next)->setActive(false);
        next++;
    }
    particlesActive_.clear();
}

void ParticleEmitterComponent::stop()
{
    emitting_ = false;
}

PropertySetBase *ParticleEmitterComponent::getProperties() const
{
    return new PropertySet<ParticleEmitterComponent>();
}

}
