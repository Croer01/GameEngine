//
// Created by adria on 10/05/2020.
//

#ifndef GAMEENGINE_PARTICLE_HPP
#define GAMEENGINE_PARTICLE_HPP

#include <game-engine/Range.hpp>
#include "GraphicHolder.hpp"

namespace GameEngine{
namespace Internal{

class Particle;
typedef std::shared_ptr<Particle> ParticleRef;

class Particle
{
    Vec2D position_;
    float size_;
    Vec2D direction_;
    bool active_;
    float timeLife_;
    Range speed_;
    Range scale_;
    ColorRange color_;
    // internal
    float lifetimeAccumulator_;

    std::shared_ptr<Internal::GraphicHolder> graphic_;
    Internal::GraphicsEngine *graphicEngine_;
public:
    Particle(Internal::GraphicsEngine *graphicEngine, const std::shared_ptr<Internal::Graphic> &graphic);;

    ~Particle();

    void setPosition(const Vec2D &position);

    void setRotation(float rotation);

    void setSize(float size);

    bool isActive() const;

    void setActive(bool active);

    void setTimeLife(float timeLife);

    void setSpeed(float min, float max);
    void setScale(float min, float max);

    void setColor(geColor start, geColor end);

    void update(float elapsedTime);
};

}
}

#endif //GAMEENGINE_PARTICLE_HPP
