//
// Created by adria on 12/10/2018.
//

#ifndef SPACEINVADERS_GRAPHICSENGINE_HPP
#define SPACEINVADERS_GRAPHICSENGINE_HPP


#include <vector>
#include <memory>
#include "GraphicHolder.hpp"
#include "Shader.h"
#include "../Singleton.hpp"
#include "../Screen.hpp"
#include "font/Text.hpp"
#include "Camera.hpp"

namespace GameEngine {
namespace Internal {
    class GraphicsEngine : public Singleton<GraphicsEngine> {
        struct InternalMesh {
            unsigned int VAO;
            unsigned int vbo;
            unsigned int ibo;
        };

        std::vector<std::shared_ptr<GraphicHolder>> graphics_;
        std::vector<std::shared_ptr<Text>> texts_;
        InternalMesh mesh_;
        std::shared_ptr<Shader> spriteShader_;
        std::shared_ptr<Shader> textShader_;
        glm::mat4 projMatrix_;
        bool pixelPerfect_;
    public:
        ~GraphicsEngine() override;

        void init(const Screen &screen);

        void draw(const std::shared_ptr<Camera> &cam);

        void registerGraphic(const std::shared_ptr<GraphicHolder> &graphic);

        void unregisterGraphic(const std::shared_ptr<GraphicHolder> &graphic);

        void registerText(const std::shared_ptr<Text> &textGraphic);

        void unregisterText(const std::shared_ptr<Text> &textGraphic);

        bool isPixelPerfect() const;
    };
}
}

#endif //SPACEINVADERS_GRAPHICSENGINE_HPP
