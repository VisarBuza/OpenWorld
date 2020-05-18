#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>


class Terrain {
  public:
    struct Vertex {
      glm::vec3 position{};
      glm::vec3 normals{};
      glm::vec2 texcoord{};
    };

    
    void load(const char* file);
    void draw(Shader shader);

    
  private:
    unsigned int vbo{};
    unsigned int vao{};
    unsigned int ebo{};

    int index_count;
    std::vector<Vertex> vertices {};
    std::vector<unsigned> indices {};
    float calculateHeight(int height);
  
};

#endif