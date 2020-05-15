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
    int numParticles = maxParticles_;
    // Add 1 extra particle to ensure always there is one particle disabled and the particle system spawn
    // particles smoothly, without cuts
    if(maxParticles_ == 0 && spawnFrequency_ != 0)
        numParticles = static_cast<int>(std::ceil(particleProperties->endLifetime() / spawnFrequency_) + 1);

    pool_.reserve(numParticles);
    for(auto i = 0; i < numParticles; i++)
    {
        pool_.push_back(particleProperties->createParticle());
    }

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
    auto *properties = new PropertySet<ParticleEmitterComponent>();

    properties->add(new Property<ParticleEmitterComponent, int>(
        "maxParticles",
        &ParticleEmitterComponent::maxParticles,
        &ParticleEmitterComponent::maxParticles,
        0
        ));

    properties->add(new Property<ParticleEmitterComponent, float>(
        "spawnFrequency",
        &ParticleEmitterComponent::spawnFrequency,
        &ParticleEmitterComponent::spawnFrequency,
        1
    ));
    properties->add(new Property<ParticleEmitterComponent, float>(
        "timeLife",
        &ParticleEmitterComponent::timeLife,
        &ParticleEmitterComponent::timeLife,
        1
    ));
    properties->add(new Property<ParticleEmitterComponent, bool>(
        "infinite",
        &ParticleEmitterComponent::infinite,
        &ParticleEmitterComponent::infinite,
        false
    ));
    properties->add(new Property<ParticleEmitterComponent, bool>(
        "emitOnInit",
        &ParticleEmitterComponent::emitOnInit,
        &ParticleEmitterComponent::emitOnInit,
        false
    ));

    return properties;
}

int ParticleEmitterComponent::maxParticles() const
{
    return maxParticles_;
}

void ParticleEmitterComponent::maxParticles(const int &value)
{
    maxParticles_ = value;
}

float ParticleEmitterComponent::spawnFrequency() const
{
    return spawnFrequency_;
}

void ParticleEmitterComponent::spawnFrequency(const float &value)
{
    spawnFrequency_ = value;
}

float ParticleEmitterComponent::timeLife() const
{
    return timeLife_;
}

void ParticleEmitterComponent::timeLife(const float &value)
{
    timeLife_ = value;
}

bool ParticleEmitterComponent::infinite() const
{
    return infinite_;
}

void ParticleEmitterComponent::infinite(const bool &value)
{
    infinite_ = value;
}

bool ParticleEmitterComponent::emitOnInit() const
{
    return emitOnInit_;
}

void ParticleEmitterComponent::emitOnInit(const bool &value)
{
    emitOnInit_ = value;
}

}
