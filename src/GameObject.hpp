//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_GAMEOBJECT_HPP
#define SPACEINVADERS_GAMEOBJECT_HPP


#include <string>
#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/detail/type_mat.hpp>
#include "events/Subject.hpp"

class Component;
enum class GameObjectEvent{
    TransformChanged
};

class GameObject : public Subject<GameObjectEvent> {
    static int ID_GENERATOR;
    int id_;
    std::string nameType_;
    std::vector<std::shared_ptr<Component>> components_;
    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::vec3 scale_;
public:

    explicit GameObject(const std::string &name);
    ~GameObject();

    void Init();

    void Update(float elapsedTime);

    std::string getType() const { return nameType_; };

    void addComponent(std::shared_ptr<Component> component);

    void fromFile(const std::string &filename);

    std::shared_ptr<GameObject> Clone() const;

    const glm::vec3 &getPosition() const;

    void setPosition(const glm::vec3 &position);

    const glm::vec3 &getRotation() const;

    void setRotation(const glm::vec3 &rotation);

    const glm::vec3 &getScale() const;

    void setScale(const glm::vec3 &scale);

    glm::mat4 getTransform();
};


#endif //SPACEINVADERS_GAMEOBJECT_HPP
