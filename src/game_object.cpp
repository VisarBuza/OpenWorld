#include "game_object.h"
#include <iostream>

void GameObject::rotate() {
  crossProduct = glm::cross(front, direction);
  dotProduct = glm::dot(front, direction);

  if (crossProduct.y < 0) {
    rotation += glm::degrees(glm::acos(dotProduct)) * (-1);
  } else {
    rotation += glm::degrees(glm::acos(dotProduct));
  }
}