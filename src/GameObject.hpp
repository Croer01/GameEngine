//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_GAMEOBJECT_HPP
#define SPACEINVADERS_GAMEOBJECT_HPP


#include <string>
#include <vector>

class Component;

class GameObject {
    static int ID_GENERATOR;
    int id_;
    std::string name_;
    std::vector<Component *> components_;
public:
    explicit GameObject(std::string name);
    ~GameObject();

    void Init();

    void Update(float elapsedTime);

    std::string GetName() const { return name_; };

    void addComponent(Component *component);

    void fromFile(const std::string &filename);

    GameObject * Clone() const;
};


#endif //SPACEINVADERS_GAMEOBJECT_HPP
