//
// Created by adria on 22/09/2018.
//

#include "Component.hpp"

void Component::SetParent(GameObject *parent) {
    parent_ = parent;
}

void Component::fromFile(const YAML::Node &componentConfig) {

}
