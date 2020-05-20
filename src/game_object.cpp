#include "game_object.h"
#include <iostream>

GameObject::GameObject(int id, std::string modelPath, std::string texturePath, glm::vec3 position)
    : id(id),
      position(position),
      nextPosition(position),
      front(glm::vec3(-1.0f, 0.0f, 0.0f)) {
  model = std::unique_ptr<Model>(new Model());
  model->load(modelPath, texturePath);
  rotation = 180;
}


void GameObject::rotate() {
  crossProduct = glm::cross(front, direction);
  dotProduct = glm::dot(front, direction);

  if (crossProduct.y < 0) {
    rotation += glm::degrees(glm::acos(dotProduct)) * (-1);
  } else {
    rotation += glm::degrees(glm::acos(dotProduct));
  }
}