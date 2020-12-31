//
// Created by adria on 12/10/2018.
//

#ifndef SPACEINVADERS_GRAPHICSENGINE_HPP
#define SPACEINVADERS_GRAPHICSENGINE_HPP


#include <vector>
#include <memory>
#include <mutex>
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
        std::vector<std::shared_ptr<GraphicHolder>> graphicsToInitialize_;
        std::vector<std::shared_ptr<Text>> texts_;
        std::shared_ptr<Shader> spriteShader_;
        std::shared_ptr<Shader> textShader_;
        glm::mat4 projMatrix_;
        bool pixelPerfect_;
        std::unique_ptr<FBO> fbo_;
        Screen *screen_;
        std::mutex graphicsToInitMutex_;

        void eraseGraphicFromList(std::vector<std::shared_ptr<GraphicHolder>> &graphics, const std::shared_ptr<GraphicHolder> &graphic);
    public:
        ~GraphicsEngine();

        GraphicsEngine(Screen *screen, bool embedded);

        void draw(Camera *cam);

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
