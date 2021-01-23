//
// Created by adria on 08/05/2020.
//

#ifndef GAMEENGINE_PARTICLESPROPERTIESCOMPONENT_HPP
#define GAMEENGINE_PARTICLESPROPERTIESCOMPONENT_HPP

#include <game-engine/geComponent.hpp>
#include <game-engine/Range.hpp>
#include <game-engine/Random.hpp>
#include <game-engine/internal/graphics/Particle.hpp>
#include <game-engine/internal/graphics/GraphicSprite.hpp>

namespace GameEngine
{

COMPONENT(ParticlesPropertiesComponent)
class ParticlesPropertiesComponent : public geComponentInstantiable<ParticlesPropertiesComponent>
{
    std::string imageSource_;
    float startSpeed_;
    float endSpeed_;
    float startScale_;
    float endScale_;
    float startOpacity_;
    float endOpacity_;
    Range rotation_;
    Range lifetime_;
    geColor startTint_;
    geColor endTint_;

    Random random_;
    Internal::GraphicsEngine *graphicsEngine_;
    std::shared_ptr<Internal::GraphicSprite> particleSprite_;
public:
    ParticlesPropertiesComponent();
    virtual void preInit();
    virtual PropertySetBase *getProperties() const;

    Internal::ParticleRef createParticle();
    void imageSource(const std::string &value);
    std::string imageSource() const;
    void minRotation(const float &value);
    float minRotation() const;
    void maxRotation(const float &value);
    float maxRotation() const;
    float startSpeed() const;
    void startSpeed(const float &value);
    float endSpeed() const;
    void endSpeed(const float &value);
    float startScale() const;
    void startScale(const float &value);
    float endScale() const;
    void endScale(const float &value);
    float startOpacity() const;
    void startOpacity(const float &value);
    float endOpacity() const;
    void endOpacity(const float &value);
    float startLifetime() const;
    void startLifetime(const float &value);
    float endLifetime() const;
    void endLifetime(const float &value);
    geColor startTint() const;
    void startTint(const geColor &value);
    geColor endTint() const;
    void endTint(const geColor &value);
};

}

#endif //GAMEENGINE_PARTICLESPROPERTIESCOMPONENT_HPP
