//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_GAMEOBJECT_HPP
#define SPACEINVADERS_GAMEOBJECT_HPP


#include <string>
#include <vector>
#include <memory>

class Component;

class GameObject {
    static int ID_GENERATOR;
    int id_;
    std::string nameType_;
    std::vector<std::shared_ptr<Component>> components_;
public:
    explicit GameObject(std::string name);
    ~GameObject();

    void Init();

    void Update(float elapsedTime);

    std::string getType() const { return nameType_; };

    void addComponent(std::shared_ptr<Component> component);

    void fromFile(const std::string &filename);

    std::shared_ptr<GameObject> Clone() const;
};


#endif //SPACEINVADERS_GAMEOBJECT_HPP
