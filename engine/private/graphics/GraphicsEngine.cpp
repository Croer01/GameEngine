//
// Created by adria on 12/10/2018.
//

#include <GL/glew.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "GraphicsEngine.hpp"
#include "../utils.hpp"
#include "font/FontManager.hpp"

namespace GameEngine {
namespace Internal {

#if _DEBUG
    // base on code from https://blog.nobel-joergensen.com/2013/02/17/debugging-opengl-part-2-using-gldebugmessagecallback/
    void openglCallbackFunction(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam){

        std::cout << "---------------------opengl-callback-start------------" << std::endl;
        std::cout << "message: "<< message << std::endl;
        std::cout << "type: ";
        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                std::cout << "ERROR";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                std::cout << "DEPRECATED_BEHAVIOR";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                std::cout << "UNDEFINED_BEHAVIOR";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                std::cout << "PORTABILITY";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                std::cout << "PERFORMANCE";
                break;
            case GL_DEBUG_TYPE_OTHER:
                std::cout << "OTHER";
                break;
        }
        std::cout << std::endl;

        std::cout << "id: " << id << std::endl;
        std::cout << "severity: ";
        switch (severity){
            case GL_DEBUG_SEVERITY_LOW:
                std::cout << "LOW";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                std::cout << "MEDIUM";
                break;
            case GL_DEBUG_SEVERITY_HIGH:
                std::cout << "HIGH";
                break;
        }
        std::cout << std::endl;
        std::cout << "---------------------opengl-callback-end--------------" << std::endl;
    }
#endif

    GraphicsEngine::GraphicsEngine(Screen *screen, bool embedded)
    {

#if _DEBUG
        if(glDebugMessageCallback) {
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(openglCallbackFunction, nullptr);
            GLuint unusedIds = 0;
            glDebugMessageControl(GL_DONT_CARE,
                                  GL_DONT_CARE,
                                  GL_DONT_CARE,
                                  0,
                                  &unusedIds,
                                  true);
        }
        else
            std::cerr << "glDebugMessageCallback not available" << std::endl;
#endif
        screen_ = screen;

        //set alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        spriteShader_ = std::make_shared<Shader>("Basic");
        spriteShader_->setVertexShader(R"EOF(
        #version 330 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;

        uniform vec2 TexOffest;
        uniform vec2 TexCoordScale;
        out vec2 TexCoord;

        uniform mat4 projView;
        uniform mat4 transform;

        void main() {
            gl_Position = projView * transform * vec4(aPos, 1.0);
            TexCoord = TexOffest + aTexCoord * TexCoordScale;
        }
        )EOF");
        spriteShader_->setFragmentShader(R"EOF(
        #version 330 core
        out vec4 FragColor;

        in vec2 TexCoord;

        uniform sampler2D Texture;
        uniform vec4 Color;

        void main() {
            vec4 texColor = texture(Texture, TexCoord);
            FragColor = texColor * Color;
        }
        )EOF");
        spriteShader_->build();
        CheckGlError();

        textShader_ = std::make_shared<Shader>("TextShader");
        textShader_->setVertexShader(R"EOF(
        #version 330 core

        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoord;

        out vec2 TexCoord;

        uniform mat4 projView;
        uniform mat4 transform;

        void main() {
            gl_Position = projView * transform * vec4(aPos, 1.0);
            TexCoord = aTexCoord;
        }
        )EOF");
        textShader_->setFragmentShader(R"EOF(
        #version 330 core
        out vec4 FragColor;

        in vec2 TexCoord;

        uniform sampler2D Texture;
        uniform vec4 Color;

        void main() {
            // use the red channel to hold letter fill information
            vec4 sampled = vec4(1.0, 1.0, 1.0, texture(Texture, TexCoord).r);
            FragColor = Color * sampled;
        }
        )EOF");
        textShader_->build();
        CheckGlError();

        //init projection matrix
        projMatrix_ = glm::ortho(0.0f, (float) screen_->virtualWidth(), (float) screen_->virtualHeight(), 0.0f, 0.f, 1.f);
        CheckGlError();

        pixelPerfect_ = screen_->pixelPerfect();
        if(embedded)
            fbo_ = std::make_unique<FBO>(screen->virtualWidth(), screen->virtualHeight());
    }

    void GraphicsEngine::draw(const std::shared_ptr<Camera> &cam)
    {
        if(fbo_)
            fbo_->bind();

        glClear(GL_COLOR_BUFFER_BIT);
        glViewport(screen_->calculatedX(), screen_->calculatedY(), screen_->calculatedWidth(),
                   screen_->calculatedHeight());

        glm::mat4 projViewMatrix = projMatrix_;

        CheckGlError();
        if(cam)
            projViewMatrix = projMatrix_ * cam->getViewMatrix();

        CheckGlError();
        for (const std::shared_ptr<GraphicHolder> &graphic : graphics_) {
            spriteShader_->bind();
            spriteShader_->setUniform("projView", projViewMatrix);
            graphic->draw(spriteShader_);
            spriteShader_->unbind();
            CheckGlError();
        }

        //draw text
        textShader_->bind();

        textShader_->setUniform("projView", projViewMatrix);
        CheckGlError();
        for (const std::shared_ptr<Text> &text : texts_) {
            text->draw(textShader_);
            CheckGlError();
        }
        textShader_->unbind();
        CheckGlError();

        if(fbo_)
            fbo_->unBind();
    }

    GraphicsEngine::~GraphicsEngine()
    {
        if(fbo_)
            fbo_->bind();

        for(auto graphic : graphics_)
        {
            graphic->setEngine(nullptr);
        }
        graphics_.clear();

        for(auto text : texts_)
        {
            text->setEngine(nullptr);
        }
        texts_.clear();

        if(fbo_)
            fbo_->unBind();

        fbo_.reset();
    }

    void GraphicsEngine::registerGraphic(const std::shared_ptr<GraphicHolder> &graphic) {
        graphics_.push_back(graphic);
        graphic->setEngine(this);
    }

    void GraphicsEngine::unregisterGraphic(const std::shared_ptr<GraphicHolder> &graphic) {
        if(graphics_.empty())
            return;

        auto it = std::find(graphics_.begin(), graphics_.end(), graphic);
        if (it != graphics_.end()) {
            (*it)->setEngine(nullptr);
            graphics_.erase(it);
        }
    }

    void GraphicsEngine::registerText(const std::shared_ptr<Text> &textGraphic) {
        texts_.push_back(textGraphic);
        textGraphic->setEngine(this);
    }


    void GraphicsEngine::unregisterText(const std::shared_ptr<Text> &textGraphic) {
        if(texts_.empty())
            return;

        auto it = std::find(texts_.begin(), texts_.end(), textGraphic);
        if (it != texts_.end()) {
            (*it)->setEngine(nullptr);
            texts_.erase(it);
        }
    }

    bool GraphicsEngine::isPixelPerfect() const {
        return pixelPerfect_;
    }

FBO *GraphicsEngine::getFbo() const
{
    return fbo_.get();
}

}
}