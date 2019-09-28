//
// Created by adria on 03/11/2018.
//

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Text.hpp"
#include "../../utils.hpp"
#include "../GraphicsEngine.hpp"
namespace GameEngine {
namespace Internal {
    Text::Text(const std::shared_ptr<Font> &font, const TextDef &textDef) :
            font_(font),
            textDef_(textDef),
            modelTransform_(glm::mat4(1)),
            active_(true) {

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

    Text::~Text() {
        glDeleteBuffers(1, &ibo);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &VAO);
    }

    void Text::draw(const std::shared_ptr<Shader> &shader) {
        if (!active_)
            return;
        shader->setUniform("transform", modelTransform_);

        shader->setAttribute(Shader::Attributes::Vertex, vbo);
        shader->setAttribute(Shader::Attributes::UV, vbo);
        shader->setAttribute(Shader::Attributes::Indices, ibo);

        glBindVertexArray(VAO);

        for(const CharDef &charDef : textDef_.characters)
        {
            if(charDef.isBreakLine)
                continue;

            glBindTexture(GL_TEXTURE_2D, charDef.textureId);

            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, charDef.vertices.size() * sizeof(GLfloat),
                            charDef.vertices.data());
            shader->draw();
        }
    }

void Text::setModelTransform(const Vec2D &position, const Vec2D &rotation, const Vec2D &scale) {
        glm::vec3 desiredPosition = glm::vec3(position.x,position.y,0.f);

        if (GraphicsEngine::GetInstance().isPixelPerfect())
            desiredPosition = glm::round(desiredPosition);

        //remember the order of matrix multiplication is from right to left
        modelTransform_ = glm::translate(glm::mat4(1), desiredPosition) * glm::mat4_cast(glm::quat(glm::vec3(rotation.x, rotation.y,0.f))) *
                          glm::scale(glm::mat4(1), glm::vec3(scale.x, scale.y, 1.f));
    }

    void Text::setActive(bool active) {
        active_ = active;
    }

    void Text::setText(const std::string &text) {
        textDef_ = font_->createTextDef(text);
    }

    int Text::width() const
    {
        return textDef_.width;
    }

    int Text::height() const
    {
        return textDef_.height;
    }

Vec2D Text::getPixelPosFromTextIndex(int index) const
{
    Vec2D pos;

    if (textDef_.characters[index].isBreakLine)
    {
        //get the Y from the previous char if it exists
        float y = 0.f;
        if(index-1 >= 0)
        {
            std::array<float, 20> vertices = textDef_.characters[index - 1].vertices;
            y = vertices[6];
        }
        pos = Vec2D(textDef_.width,y);
    }
    else
    {
        std::array<float,20> vertices = textDef_.characters[index].vertices;
        //get the position of the top-left vertex
        pos = Vec2D(vertices[5],vertices[6]);
    }
    return pos;
}

}
}