//
// Created by adria on 31/10/2018.
//

#ifndef SPACEINVADERS_PHYSICSENGINE_HPP
#define SPACEINVADERS_PHYSICSENGINE_HPP


#include <memory>
#include <Box2D/Box2D.h>
#include <unordered_map>
#include "../Singleton.hpp"
#include "Collider.hpp"
#ifdef DEBUG
#include "DebugView.hpp"
#include "../graphics/Camera.hpp"

#endif
namespace GameEngine {
namespace Internal {
    class PhysicsEngine : public Singleton<PhysicsEngine>, b2ContactListener {

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

    public:
        static float getScalePixelsToMeter() { return 32.f; }

        void init(float timeStep);

        void createCategories(const std::vector<std::string> &categories);

        void createMasks(const std::unordered_map<std::string, std::vector<std::string>> &masks);

        void update(float elapsedTime);

        void drawDebug(const std::shared_ptr<Camera> &cam);

        void registerCollider(const std::shared_ptr<Collider> &collider);

        void unregisterCollider(const std::shared_ptr<Collider> &collider);
    };
}
}

#endif //SPACEINVADERS_PHYSICSENGINE_HPP
