//
// Created by adria on 01/11/2018.
//

#include "DebugView.hpp"
#include "PhysicsEngine.hpp"

#include <GL\glew.h>
namespace GameEngine {
namespace Internal {
//TODO: fill the rest of the methods
    void DebugView::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
        beginDraw();

        // Standard OpenGL rendering stuff
        glColor4f(color.r, color.g, color.b, color.a);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBegin(GL_POLYGON);
        for (int i = 0; i < vertexCount; i++) {
            glVertex2f(vertices[i].x, vertices[i].y);
        }
        glEnd();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        endDraw();
    }

// We just need to have these to prevent override errors, they don't actually do anything right now
    void DebugView::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {}

    void DebugView::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color) {}

    void DebugView::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) {}

    void DebugView::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {}

    void DebugView::DrawTransform(const b2Transform &xf) {}

    void DebugView::DrawPoint(const b2Vec2 &p, float32 size, const b2Color &color) {}

    void DebugView::beginDraw() {
        glPushMatrix(); // Push matrix so we can revert after doing some transformations
        //TODO: set the size of the camera from virtual screen
        glOrtho(0.0f, 224, 256, 0, 0.f, 1.f);
        glScalef(PhysicsEngine::getScalePixelsToMeter(), PhysicsEngine::getScalePixelsToMeter(), 1);
    }


    void DebugView::endDraw() {
        glPopMatrix(); // Revert transformations
    }
}
}