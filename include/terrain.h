#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

class Terrain {
 public:
  ~Terrain() noexcept;
  Terrain(const Terrain&) = delete;
  Terrain& operator=(const Terrain&) = delete;
  Terrain() = default;

  struct Vertex {
    glm::vec3 position{};
    glm::vec3 normals{};
    glm::vec2 texcoord{};
  };

  void load(const char *file, const char *textureFile);
  void draw(Shader shader);

 private:
  unsigned int vbo{};
  unsigned int vao{};
  unsigned int ebo{};
  Texture texture;

  int index_count;
  std::vector<Vertex> vertices{};
  std::vector<unsigned> indices{};
  float calculateHeight(int height);
  void readHeightMap(const char *file);
  void setGraphicsData();
  glm::vec3 calculateNormal(int x, int z, unsigned char* image);
};

#endif