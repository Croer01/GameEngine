//
// Created by adria on 22/09/2018.
//

#include <iostream>
#include "PrintTextComponent.hpp"

void PrintTextComponent::Update(float elapsedTime) {
std::cout << "parent name " << parent_->GetName() << std::endl;
}

std::shared_ptr<Component> PrintTextComponent::Clone() {
    return std::make_shared<PrintTextComponent>();
}
