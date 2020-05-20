#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <map>

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
  void loadSkybox();
  void draw(Shader shader);
  void drawSkybox(Shader shader, glm::mat4 view, glm::mat4 projection);
  void update(float dt);

  float getHeight(float worldX, float worldZ);

 private:
  unsigned int vbo{};
  unsigned int vao{};
  unsigned int ebo{};
  unsigned int skyboxVBO{};
  unsigned int skyboxVAO{};
  unsigned int cubemapTextureDay{};
  unsigned int cubeMapTextureNight{};

  Texture texture;

  int index_count;
  std::vector<Vertex> vertices{};
  std::vector<unsigned> indices{};
  std::map<std::pair<int, int>, float> heights;
  float rotation = 0;
  float time = 0;
  float blendFactor = 0;
  glm::vec3 ambient{};
  glm::vec3 diffuse{};
  glm::vec3 specular{};
  glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

  float calculateHeight(int height);
  void readHeightMap(const char *file);
  void setGraphicsData();
  glm::vec3 calculateNormal(int x, int z, unsigned char* image);
};

#endif