//
// Created by adria on 03/11/2018.
//

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Text.hpp"
#include "../../utils.hpp"
#include "../GraphicsEngine.hpp"

Text::Text(const TextDef &textDef) :
    textDef_(textDef),
    modelTransform_(glm::mat4(1))
{

    glGenVertexArrays(1, &VAO);

    //  Set vertex data
    glBindVertexArray(VAO);

    //Create VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 20, nullptr, GL_DYNAMIC_DRAW);

    //Create IBO
    //IBO data
    std::vector<unsigned int> indices = std::vector<unsigned int>({
        0, 1, 2,
        0, 2, 3
    });
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STREAM_DRAW);

    glBindVertexArray(0);
    CheckGlError();
}

void Text::draw(const std::shared_ptr<Shader> &shader) {
    shader->setUniform("transform", modelTransform_);

    shader->setAttribute(Shader::Attributes::Vertex, vbo);
    shader->setAttribute(Shader::Attributes::UV, vbo);
    shader->setAttribute(Shader::Attributes::Indices, ibo);

    assert(textDef_.textureIds.size() == textDef_.verticesByCharacter.size());

    for (auto i = 0; i < textDef_.textureIds.size(); i++) {
        glBindTexture(GL_TEXTURE_2D, textDef_.textureIds[i]);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, textDef_.verticesByCharacter[i].size() * sizeof(GLfloat), textDef_.verticesByCharacter[i].data());
        shader->draw();
    }
}

void Text::setModelTransform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) {
    //remember the order of matrix multiplication is from right to left
    if(GraphicsEngine::GetInstance().isPixelPerfect())
        modelTransform_ = glm::translate(glm::mat4(1), glm::round(position)) * glm::mat4_cast(glm::quat(rotation)) * glm::scale(glm::mat4(1), scale);
    else
        modelTransform_ = glm::translate(glm::mat4(1), position) * glm::mat4_cast(glm::quat(rotation)) * glm::scale(glm::mat4(1), scale);
}

Text::~Text() {
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &VAO);
}
