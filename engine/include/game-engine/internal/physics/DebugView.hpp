//
// Created by adria on 01/11/2018.
//

#ifndef SPACEINVADERS_DEBUGVIEW_HPP
#define SPACEINVADERS_DEBUGVIEW_HPP

#include <Box2D\Box2D.h>
#include <game-engine/internal/graphics/Shader.h>
#include <game-engine/internal/graphics/Camera.hpp>
#include <game-engine/internal/Screen.hpp>

namespace GameEngine {
namespace Internal {
    class DebugView : public b2Draw {
        std::shared_ptr<Shader> shader_;
        Camera *cam_;
        Screen *screen_;

        // common instructions required before draw something
        void beginDraw();

        // common instructions required after draw something
        void endDraw();

        void DrawCircleInternal(const b2Vec2 &center, float32 radius, const b2Color &color);
    public:
        explicit DebugView(Screen *screen);
        // We won't be implementing all of these, but if we don't declare them here we'll get an override error
        void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;

        void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;

        void DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) override;

        void DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) override;

        void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override;

        void DrawTransform(const b2Transform &xf) override;

        void setCamera(Camera *cam);
    };
}
}

#endif //SPACEINVADERS_DEBUGVIEW_HPP
