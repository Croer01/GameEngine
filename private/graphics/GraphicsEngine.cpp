//
// Created by adria on 12/10/2018.
//

#include <GL/glew.h>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "GraphicsEngine.hpp"
#include "../utils.hpp"
#include "font/FontManager.hpp"

namespace GameEngine {
namespace Internal {
    void GraphicsEngine::init(const Screen &screen) {

        //set alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //The order of the vertices are fliped to from "n" to "u" way to deal with the inverted y axis
        //VBO data
        std::vector<float> vertices = std::vector<float>({ //vertex(3) | uv(2)
                                                                 0.f, 1.f, 0.f, 0.f, 1.f,
                                                                 0.f, 0.f, 0.f, 0.f, 0.f,
                                                                 1.f, 0.f, 0.f, 1.f, 0.f,
                                                                 1.f, 1.f, 0.f, 1.f, 1.f
                                                         });
        //IBO data
        std::vector<unsigned int> indices = std::vector<unsigned int>({
                                                                              0, 1, 2,
                                                                              0, 2, 3
                                                                      });


        glGenVertexArrays(1, &mesh_.VAO);

        //  Set vertex data
        glBindVertexArray(mesh_.VAO);

        //Create VBO
        glGenBuffers(1, &mesh_.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, mesh_.vbo);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices.front(), GL_STREAM_DRAW);

        //Create IBO
        glGenBuffers(1, &mesh_.ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_.ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STREAM_DRAW);
        CheckGlError();

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
            FragColor = vec4(texColor.rgb * Color.rgb, texColor.a);
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
        projMatrix_ = glm::ortho(0.0f, (float) screen.virtualWidth(), (float) screen.virtualHeight(), 0.0f, 0.f, 1.f);
        CheckGlError();

        pixelPerfect_ = screen.pixelPerfect();
    }

    void GraphicsEngine::draw(const std::shared_ptr<Camera> &cam) {
        glm::mat4 projViewMatrix = projMatrix_;

        if(cam)
            projViewMatrix = projMatrix_ * cam->getViewMatrix();

        spriteShader_->bind();

        spriteShader_->setUniform("projView", projViewMatrix);

        spriteShader_->setAttribute(Shader::Attributes::Vertex, mesh_.vbo);
        spriteShader_->setAttribute(Shader::Attributes::UV, mesh_.vbo);
        spriteShader_->setAttribute(Shader::Attributes::Indices, mesh_.ibo);

        for (const std::shared_ptr<GraphicHolder> &graphic : graphics_) {
            graphic->draw(spriteShader_);
        }

        spriteShader_->unbind();

        //draw text
        textShader_->bind();

        textShader_->setUniform("projView", projViewMatrix);

        for (const std::shared_ptr<Text> &text : texts_) {
            text->draw(textShader_);
        }

        textShader_->unbind();
    }

    GraphicsEngine::~GraphicsEngine() {

        glDeleteBuffers(1, &mesh_.ibo);
        glDeleteBuffers(1, &mesh_.vbo);
        glDeleteVertexArrays(1, &mesh_.VAO);
    }

    void GraphicsEngine::registerGraphic(const std::shared_ptr<GraphicHolder> &graphic) {
        graphics_.push_back(graphic);
    }

    void GraphicsEngine::unregisterGraphic(const std::shared_ptr<GraphicHolder> &graphic) {
        auto it = std::find(graphics_.begin(), graphics_.end(), graphic);
        if (it != graphics_.end()) {
            std::swap(*it, *(--graphics_.end()));
            graphics_.pop_back();
        }
    }

    void GraphicsEngine::registerText(const std::shared_ptr<Text> &textGraphic) {
        texts_.push_back(textGraphic);
    }


    void GraphicsEngine::unregisterText(const std::shared_ptr<Text> &textGraphic) {
        auto it = std::find(texts_.begin(), texts_.end(), textGraphic);
        if (it != texts_.end()) {
            std::swap(*it, *(--texts_.end()));
            texts_.pop_back();
        }
    }

    bool GraphicsEngine::isPixelPerfect() const {
        return pixelPerfect_;
    }
}
}