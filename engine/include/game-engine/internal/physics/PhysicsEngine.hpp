//
// Created by adria on 31/10/2018.
//

#ifndef SPACEINVADERS_PHYSICSENGINE_HPP
#define SPACEINVADERS_PHYSICSENGINE_HPP


#include <game-engine/api.hpp>
#include <memory>
#if defined _WIN32
#  include <Box2D/Box2D.h>
#else
#  include <box2d/box2d.h>
#endif
#include <unordered_map>
#include <game-engine/internal/physics/Collider.hpp>
#ifdef DEBUG
#include <game-engine/internal/physics/DebugView.hpp>
#include <game-engine/internal/graphics/Camera.hpp>
#include <game-engine/internal/Screen.hpp>
#endif
namespace GameEngine {
namespace Internal {
    class PUBLICAPI PhysicsEngine : b2ContactListener {

        struct ColliderCategory {
            uint16 categoryBit = 0;
            uint16 maskBits = std::numeric_limits<uint16>::max();
        };

        float timeStep_;
        float timeAcumulator_;
        int velocityIterations_;
        int positionIterations_;
        std::unique_ptr<b2World> world_;
        std::vector<std::shared_ptr<Collider>> colliders_;
        std::unordered_map<std::string, ColliderCategory> categories_;

#ifdef DEBUG
        std::unique_ptr<DebugView> debugView_;
#endif

        void BeginContact(b2Contact *contact) override;
        void EndContact(b2Contact *contact) override;

public:
        ~PhysicsEngine();

        static float getScalePixelsToMeter() { return 32.f; }

#ifdef DEBUG
        void init(float timeStep, Screen *screen);
#endif
        void init(float timeStep);

        void createCategories(const std::vector<std::string> &categories);

        void createMasks(const std::unordered_map<std::string, std::vector<std::string>> &masks);

        void update(float elapsedTime);

#ifdef DEBUG
        void drawDebug(Camera *cam);
#endif
        void registerCollider(const std::shared_ptr<Collider> &collider);

        void unregisterCollider(const std::shared_ptr<Collider> &collider);
        b2Joint *createRevoluteJoin(b2Body *bodyA, b2Body *bodyB, float xBodyA, float yBodyA);
    };
}
}

#endif //SPACEINVADERS_PHYSICSENGINE_HPP
