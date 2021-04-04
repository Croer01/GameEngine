//
// Created by adria on 03/05/2020.
//

#ifndef GAMEENGINE_PARTICLEEMITTERCOMPONENT_HPP
#define GAMEENGINE_PARTICLEEMITTERCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <game-engine/geGameObject.hpp>
#include <GL/glew.h>
#include <list>
#include <iostream>
#include <memory>
#include <game-engine/internal/graphics/Particle.hpp>
#include <game-engine/components/ParticlesPropertiesComponent.hpp>
#include <game-engine/components/ComponentData.hpp>

namespace GameEngine
{
class ParticleEmitterComponentData : public ComponentData
{
public:
    ParticleEmitterComponentData()
    {
        createProperty<int>("maxParticles", 0);
        createProperty<float>("spawnFrequency", 1);
        createProperty<float>("timeLife", 1);
        // If "infinite" is true, the emitter never stop by timeLife
        createProperty<bool>("infinite", false);
        createProperty<bool>("emitOnInit", false);
    }
};

class ParticleEmitterComponent : public geComponentInstantiable<ParticleEmitterComponent, ParticleEmitterComponentData>
{
    // the origin of the particle emitter is the position of the owner object

    // internally variables to do all work
    std::weak_ptr<ParticlesPropertiesComponent> particleProperties_;
    bool emitting_;
    float spawnTimeAccumulator_;
    float timeLifeAccumulator_;
    std::vector<Internal::ParticleRef> pool_;
    std::list<Internal::ParticleRef> particlesActive_;

    void spawnParticle();

public:
    void cleanAndEmit();
    void emit();
    void stop();
    bool isEmitting() const;
    virtual void Update(float elapsedTime);
    virtual void init();
};

}


#endif //GAMEENGINE_PARTICLEEMITTERCOMPONENT_HPP
