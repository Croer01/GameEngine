//
// Created by adria on 03/05/2020.
//

#ifndef GAMEENGINE_PARTICLEEMITTERCOMPONENT_HPP
#define GAMEENGINE_PARTICLEEMITTERCOMPONENT_HPP


#include <game-engine/geComponent.hpp>
#include <GL/glew.h>
#include <list>
#include <iostream>
#include <memory>
#include "../../../private/graphics/Particle.hpp"

namespace GameEngine
{

//TODO: punt de referencia https://pixijs.io/pixi-particles-editor/#
class ParticleEmitterComponent : public geComponentInstantiable<ParticleEmitterComponent>
{
    // the origin is the position of the owner object
    float maxParticles_;
    float spawnFrequency_;
    float timeLife_;
    // If this is true, the emitter never stop by timeLife
    bool infinite_;
    bool emitOnInit_;

    // internally variables to do all work
    bool emitting_;
    float spawnTimeAccumulator_;
    float timeLifeAccumulator_;
    std::vector<Internal::ParticleRef> pool_;
    std::list<Internal::ParticleRef> particlesActive_;

    void spawnParticle();

public:
    void emit();
    void stop();
    virtual void Update(float elapsedTime);
    virtual void preInit();
    virtual PropertySetBase *getProperties() const;
};

}


#endif //GAMEENGINE_PARTICLEEMITTERCOMPONENT_HPP
