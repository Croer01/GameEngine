//
// Created by adria on 12/10/2018.
//

#ifndef SPACEINVADERS_GRAPHICSENGINE_HPP
#define SPACEINVADERS_GRAPHICSENGINE_HPP


#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <game-engine/geEnvironment.hpp>
#include <game-engine/internal/graphics/GraphicHolder.hpp>
#include <game-engine/internal/graphics/GraphicSprite.hpp>
#include <game-engine/internal/graphics/GraphicGeometry.hpp>
#include <game-engine/internal/graphics/Shader.hpp>
#include <game-engine/internal/Screen.hpp>
#include <game-engine/internal/graphics/font/Text.hpp>
#include <game-engine/internal/graphics/Camera.hpp>
#include <game-engine/internal/graphics/MeshData.hpp>
#include <game-engine/internal/graphics/FBO.hpp>

namespace GameEngine {
namespace Internal {
    class GraphicsEngine : public Observer<int, int> {
        // This is a cache of loaded graphics: sprites, geometry, etc.
        std::map<std::string, std::shared_ptr<Graphic>> cache_;
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

        std::shared_ptr<GraphicSprite> loadSprite(const std::string &filePath);

        std::shared_ptr<GraphicGeometry> loadGeometry(const std::vector<GameEngine::Vec2D> &points);

        void registerGraphic(const std::shared_ptr<GraphicHolder> &graphic);

        void unregisterGraphic(const std::shared_ptr<GraphicHolder> &graphic);

        void registerText(const std::shared_ptr<Text> &textGraphic);

        void unregisterText(const std::shared_ptr<Text> &textGraphic);

        bool isPixelPerfect() const;

        FBO *getFbo() const;

        virtual void onEvent(const Subject<int, int> &target, int width, int height);
    };
}
}

#endif //SPACEINVADERS_GRAPHICSENGINE_HPP
