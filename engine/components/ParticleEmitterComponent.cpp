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

    // if the maxParticles is set to 0, this is calculate with the maximum number of possible particles can be spawn per second
    int &maxParticles = getPropertyValue<int>("maxParticles");
    int numParticles = maxParticles;
    // Add 1 extra particle to ensure always there is one particle disabled and the particle system spawn
    // particles smoothly, without cuts
    float spawnFrequency = getPropertyValue<float>("spawnFrequency");
    if(maxParticles == 0 && spawnFrequency != 0.f)
    {
        float endLifetime = particleProperties->getPropertyValue<float>("endLifetime");
        numParticles = static_cast<int>(std::ceil(endLifetime / spawnFrequency) + 1);
    }

    pool_.reserve(numParticles);
    for(auto i = 0; i < numParticles; i++)
    {
        pool_.push_back(particleProperties->createParticle());
    }

    if(getPropertyValue<bool>("emitOnInit"))
        emit();
}

void ParticleEmitterComponent::Update(float elapsedTime)
{
    // first calculate and check if the emitter can continue spawning particles
    if(emitting_ && !getPropertyValue<bool>("infinite"))
    {
        timeLifeAccumulator_ += elapsedTime;
        if(timeLifeAccumulator_ >= getPropertyValue<float>("timeLife"))
        {
            emitting_ = false;
            timeLifeAccumulator_ = 0;
        }
    }

    // check if the emitter can spawn a new particle
    if(emitting_)
    {
        spawnTimeAccumulator_ += elapsedTime;

        float spawnFrequency = getPropertyValue<float>("spawnFrequency");
        // maybe the emitter can spawn particles faster than the elapsed time
        while (spawnTimeAccumulator_ >= spawnFrequency)
        {
            spawnParticle();
            spawnTimeAccumulator_ -= spawnFrequency;
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
}

void ParticleEmitterComponent::cleanAndEmit()
{
    emit();

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

bool ParticleEmitterComponent::isEmitting() const
{
    return emitting_;
}

}
