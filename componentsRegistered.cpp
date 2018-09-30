#include "src/ObjectManager.hpp"
#include "PrintTextComponent.hpp"

/*
 * This file has been generated. To register a component use macro COMPONENT(NameOfComponentClass)
 * in the definition of the class
 */
void RegisterComponents() {
ObjectManager::GetInstance().registerComponentBuilder("PrintTextComponent", new ComponentTBuilder<PrintTextComponent>());
}
