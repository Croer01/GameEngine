//
// Created by adria on 22/09/2018.
//

#include <iostream>
#include "PrintTextComponent.hpp"

void PrintTextComponent::Update(float elapsedTime) {
std::cout << "parent name " << text_ << std::endl;
}

std::shared_ptr<Component> PrintTextComponent::Clone() {

    std::shared_ptr<PrintTextComponent> clone = std::make_shared<PrintTextComponent>();
    clone->text_ = text_;
    return clone;
}

void PrintTextComponent::fromFile(const INIReader &iniFile) {
    text_ = iniFile.Get("PrintTextComponent","text","[Text not found]");
}
