//
// Created by adria on 17/11/2018.
//

#ifndef SPACEINVADERS_STARTMENUCOMPONENT_HPP
#define SPACEINVADERS_STARTMENUCOMPONENT_HPP


#include "src/Component.hpp"
#include "glm/glm.hpp"

COMPONENT(StartMenuComponent)
class StartMenuComponent : public Component {
    glm::vec4 rectangle_;
public:
    std::shared_ptr<Component> Clone() override;

    void Update(float elapsedTime) override;
};

#endif //SPACEINVADERS_STARTMENUCOMPONENT_HPP
