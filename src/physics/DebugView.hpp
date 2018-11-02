//
// Created by adria on 01/11/2018.
//

#ifndef SPACEINVADERS_DEBUGVIEW_HPP
#define SPACEINVADERS_DEBUGVIEW_HPP

#include <Box2D\Box2D.h>

class DebugView : public b2Draw {
    void beginDraw();
    void endDraw();

public:
    // We won't be implementing all of these, but if we don't declare them here we'll get an override error
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
    void DrawTransform(const b2Transform& xf) override;
    void DrawPoint(const b2Vec2 &p, float32 size, const b2Color &color) override;
};


#endif //SPACEINVADERS_DEBUGVIEW_HPP