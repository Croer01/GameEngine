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
#include "../../../private/graphics/Particle.hpp"
#include "ParticlesPropertiesComponent.hpp"

namespace GameEngine
{

class ParticleEmitterComponent : public geComponentInstantiable<ParticleEmitterComponent>
{
    // the origin is the position of the owner object
    int maxParticles_;
    float spawnFrequency_;
    float timeLife_;
    // If this is true, the emitter never stop by timeLife
    bool infinite_;
    bool emitOnInit_;

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
    virtual PropertySetBase *getProperties() const;

    int maxParticles() const;
    void maxParticles(const int &value);
    float spawnFrequency() const;
    void spawnFrequency(const float &value);
    float timeLife() const;
    void timeLife(const float &value);
    bool infinite() const;
    void infinite(const bool &value);
    bool emitOnInit() const;
    void emitOnInit(const bool &value);
};

}


#endif //GAMEENGINE_PARTICLEEMITTERCOMPONENT_HPP
