//
// Created by adria on 03/05/2020.
//

#include <game-engine/components/ParticleEmitterComponent.hpp>

namespace GameEngine
{

void ParticleEmitterComponent::init()
{
    particleProperties_ = gameObject()->getComponent<ParticlesPropertiesComponent>();
    auto particleProperties = particleProperties_.lock();
   if(!particleProperties)
       throw std::runtime_error("The GameObject doesn't have a ParticlesPropertiesComponent attached");

    maxParticles_ = 81;
    pool_.reserve(maxParticles_);

    for(auto i = 0; i < maxParticles_; i++)
    {
        pool_.push_back(particleProperties->createParticle());
    }

    //TODO: remove this
    emitOnInit_ = true;
    infinite_ = true;
    spawnFrequency_ = 0.01f;
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
//    std::cout << "particles active: " << particlesActive_.size() << std::endl;
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
