#include "src/ObjectManager.hpp"
#include "EnemyComponent.hpp"


/*
 * This file has been generated. To register a component use macro COMPONENT(NameOfComponentClass)
 * in the definition of the class
 */
void RegisterComponents() {
    ObjectManager::GetInstance().registerComponentBuilder("EnemyComponent", new ComponentTBuilder<EnemyComponent>());

}
