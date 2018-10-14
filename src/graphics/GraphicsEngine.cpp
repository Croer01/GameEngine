//
// Created by adria on 12/10/2018.
//

#include <GL/glew.h>
#include <glm/detail/type_mat.hpp>
#include "GraphicsEngine.hpp"

void GraphicsEngine::init() {

    //VBO data
    std::vector<float> vertices = std::vector<float>({ //vertex(3) | color(2)
                                                              -0.5f, -0.5f, 0.5f,   0.f, 1.f,
                                                              0.5f, -0.5f, 0.5f,    1.f, 1.f,
                                                              -0.5f, 0.5f, 0.5f,    0.f, 0.f,
                                                              0.5f, 0.5f, 0.5f,     1.f, 0.f
                                                      });
    //IBO data
    std::vector<unsigned int> indices = std::vector<unsigned int>({
                                                                           0, 1, 2,
                                                                           2, 1, 3,
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

    spriteShader_ = std::make_shared<Shader>("Basic");
    spriteShader_->setVertexShader(R"EOF(
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
    spriteShader_->setFragmentShader(R"EOF(
        #version 330 core
        out vec4 FragColor;

        in vec2 TexCoord;

        uniform sampler2D Texture;
        uniform vec4 Color;

        void main() {
            FragColor = texture(Texture, TexCoord) * Color;
        }
        )EOF");
    spriteShader_->build();
}

void GraphicsEngine::draw() {
    spriteShader_->bind();

    spriteShader_->setUniform("projView", glm::mat4(1));

    spriteShader_->setAttribute(Shader::Attributes::Vertex, mesh_.vbo);
    spriteShader_->setAttribute(Shader::Attributes::UV, mesh_.vbo);
    spriteShader_->setAttribute(Shader::Attributes::Indices, mesh_.ibo);

    for (const auto &graphic : graphics_) {
        graphic->draw(spriteShader_);
    }

    spriteShader_->unbind();
}

GraphicsEngine::~GraphicsEngine() {

    glDeleteBuffers(1, &mesh_.ibo);
    glDeleteBuffers(1, &mesh_.vbo);
    glDeleteVertexArrays(1, &mesh_.VAO);
}

void GraphicsEngine::registerGraphic(std::shared_ptr<Graphic> graphic) {
    graphics_.push_back(graphic);
}

