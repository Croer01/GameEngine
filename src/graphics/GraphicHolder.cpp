//
// Created by adria on 27/10/2018.
//

#include "GraphicHolder.hpp"
#include "GraphicsEngine.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

GraphicHolder::GraphicHolder(const std::shared_ptr<Graphic> &graphic) {
    graphic_ = graphic;
    modelTransform_ = glm::mat4(1);
}

void GraphicHolder::draw(const std::shared_ptr<Shader> &shader) {
    shader->setUniform("transform", modelTransform_);
    graphic_->draw(shader);
}

void GraphicHolder::setModelTransform(const glm::vec3 &position,const glm::vec3 &rotation ,const glm::vec3 &scale) {
    //remember the order of matrix multiplication is from right to left
    if(GraphicsEngine::GetInstance().isPixelPerfect())
        modelTransform_ = glm::translate(glm::mat4(1), glm::round(position)) * glm::mat4_cast(glm::quat(rotation)) * glm::scale(glm::mat4(1), scale);
    else
        modelTransform_ = glm::translate(glm::mat4(1), position) * glm::mat4_cast(glm::quat(rotation)) * glm::scale(glm::mat4(1), scale);
}

void GraphicHolder::setGraphic(const std::shared_ptr<Graphic> &graphic) {
    graphic_ = graphic;
}

std::shared_ptr<Graphic> GraphicHolder::getGraphic() const {
    return graphic_;
}

