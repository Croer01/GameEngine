//
// Created by adria on 22/09/2018.
//

#include <iostream>
#include "PrintTextComponent.hpp"

void PrintTextComponent::Update(float elapsedTime) {
std::cout << "parent name " << parent_->GetName() << std::endl;
}

Component *PrintTextComponent::Clone() {
    return new PrintTextComponent();
}
