//
// Created by adria on 27/10/2018.
//

#include "GraphicHolder.hpp"

GraphicHolder::GraphicHolder(const std::shared_ptr<Graphic> &graphic) {
    graphic_ = graphic;
    modelTransform_ = glm::mat4(1);
}

void GraphicHolder::draw(const std::shared_ptr<Shader> &shader) {
    shader->setUniform("transform", modelTransform_);
    graphic_->draw(shader);
}

void GraphicHolder::setModelTransform(const glm::mat4 &transform) {
    modelTransform_ = transform;
}

void GraphicHolder::setGraphic(const std::shared_ptr<Graphic> &graphic) {
    graphic_ = graphic;
}

std::shared_ptr<Graphic> GraphicHolder::getGraphic() const {
    return graphic_;
}

