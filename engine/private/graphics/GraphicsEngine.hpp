//
// Created by adria on 12/10/2018.
//

#ifndef SPACEINVADERS_GRAPHICSENGINE_HPP
#define SPACEINVADERS_GRAPHICSENGINE_HPP


#include <vector>
#include <memory>
#include <game-engine/geEnvironment.hpp>
#include "GraphicHolder.hpp"
#include "Shader.h"
#include "../Screen.hpp"
#include "font/Text.hpp"
#include "Camera.hpp"
#include "MeshData.hpp"
#include "FBO.hpp"

namespace GameEngine {
namespace Internal {
    class GraphicsEngine {
        std::vector<std::shared_ptr<GraphicHolder>> graphics_;
        std::vector<std::shared_ptr<Text>> texts_;
        std::shared_ptr<Shader> spriteShader_;
        std::shared_ptr<Shader> textShader_;
        glm::mat4 projMatrix_;
        bool pixelPerfect_;
        std::unique_ptr<FBO> fbo_;
        Screen *screen_;
    public:
        ~GraphicsEngine();

        GraphicsEngine(Screen *screen, bool embedded);

        void draw(const std::shared_ptr<Camera> &cam);

        void registerGraphic(const std::shared_ptr<GraphicHolder> &graphic);

        void unregisterGraphic(const std::shared_ptr<GraphicHolder> &graphic);

        void registerText(const std::shared_ptr<Text> &textGraphic);

        void unregisterText(const std::shared_ptr<Text> &textGraphic);

        bool isPixelPerfect() const;

        FBO *getFbo() const;
    };
}
}

#endif //SPACEINVADERS_GRAPHICSENGINE_HPP
