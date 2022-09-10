//
// Created by adria on 08/05/2020.
//

#ifndef GAMEENGINE_PARTICLESPROPERTIESCOMPONENT_HPP
#define GAMEENGINE_PARTICLESPROPERTIESCOMPONENT_HPP

#include <game-engine/api.hpp>
#include <game-engine/geComponent.hpp>
#include <game-engine/Range.hpp>
#include <game-engine/Random.hpp>
#include <game-engine/internal/graphics/Particle.hpp>
#include <game-engine/internal/graphics/GraphicSprite.hpp>
#include <game-engine/components/ComponentData.hpp>

namespace GameEngine
{
class PUBLICAPI ParticlesPropertiesComponentData : public ComponentData
{
public:
    ParticlesPropertiesComponentData()
    {
        createPropertyFilePath("imageSource", "", FileType::IMAGE, true);
        // Rotation is in degrees
        createProperty<float>("minRotation", 0);
        createProperty<float>("maxRotation", 0);

        createProperty<float>("startSpeed", 10);
        createProperty<float>("endSpeed", 10);

        createProperty<float>("startScale", 1);
        createProperty<float>("endScale", 1);

        createProperty<float>("startOpacity", 1);
        createProperty<float>("endOpacity", 1);

        createProperty<float>("startLifetime", 1);
        createProperty<float>("endLifetime", 1);

        createProperty<geColor>("startTint", geColor(1.f));
        createProperty<geColor>("endTint", geColor(1.f));
    }
};

class PUBLICAPI ParticlesPropertiesComponent : public geComponentInstantiable<ParticlesPropertiesComponent, ParticlesPropertiesComponentData>
{
    Range rotation_;
    Range lifetime_;

    Random random_;
    Internal::GraphicsEngine *graphicsEngine_;
    std::shared_ptr<Internal::GraphicSprite> particleSprite_;

    void updateSprite();
public:
    ParticlesPropertiesComponent();
    virtual void preInit();

    Internal::ParticleRef createParticle();
};

}

#endif //GAMEENGINE_PARTICLESPROPERTIESCOMPONENT_HPP
