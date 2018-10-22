//
// Created by adria on 12/10/2018.
//

#include <GL/glew.h>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GraphicsEngine.hpp"
#include "../utils.hpp"

void GraphicsEngine::init(int width, int height) {

    //set alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //VBO data
    std::vector<float> vertices = std::vector<float>({ //vertex(3) | uv(2)
                                                              -0.5f,  0.5f, 0.f,   0.f, 1.f,
                                                              -0.5f, -0.5f, 0.f,   0.f, 0.f,
                                                               0.5f, -0.5f, 0.f,   1.f, 0.f,
                                                               0.5f,  0.5f, 0.f,   1.f, 1.f
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
            vec4 texColor = texture(Texture, TexCoord);
            FragColor = vec4(texColor.rgb * Color.rgb, texColor.a);
        }
        )EOF");
    spriteShader_->build();
    CheckGlError();

    //init projection matrix
    projMatrix_ = glm::ortho(0.0f, (float) width, (float) height, 0.0f, 0.f,1.f) * glm::mat4(1);
                  //          glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f)
//                  * glm::lookAt(
//            glm::vec3(0, 0, 1), // Camera is at (4,3,3), in World Space
//            glm::vec3(0, 0, 0), // and looks at the origin
//            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
//    );
    CheckGlError();

}

void GraphicsEngine::draw() {
    spriteShader_->bind();

    spriteShader_->setUniform("projView", projMatrix_);

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

