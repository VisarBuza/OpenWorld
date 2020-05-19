#include "terrain.h"
#include <gfx.h>
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

/** Constants for vertex attribute locations */
constexpr auto va_position = 0;
constexpr auto va_normal = 1;
constexpr auto va_texcoord = 2;

/** Constants for texture sampler bindings */
constexpr auto tb_diffuse = 0;

Terrain::~Terrain() noexcept
{
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &vao);
}

void Terrain::load(const char *file, const char *textureFile) {
  texture.load_texture(textureFile);
  readHeightMap(file);
  setGraphicsData();
}

void Terrain::readHeightMap(const char *file) {
  int width, height, channels;
  unsigned char *image = stbi_load(file, &width, &height, &channels, STBI_rgb);

  int offset = 0;
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      int index = (x + (y * height)) * 3;
      float yHeight = calculateHeight((int)image[index]);
      vertices.push_back({});
      auto &vertex = vertices.back();
      vertex.position = glm::vec3(x - 540, yHeight, y - 540);
      vertex.normals = calculateNormal(x, y, image);
      vertex.texcoord = glm::vec2(x / 255.0, y / 255.0);
    }
  }

  int numStripsRequired = height - 1;
  int numDegensRequired = 2 * (numStripsRequired - 1);
  int verticesPerStrip = 2 * width;

  for (int y = 0; y < height - 1; y++) {
    if (y > 0) {
      indices.push_back(y * height);
    }
    for (int x = 0; x < width; x++) {
      indices.push_back((y * height) + x);
      indices.push_back(((y + 1) * height) + x);
    }
    if (y < height - 2) {
      indices.push_back(((y + 1) * height) + (width - 1));
    }
  }

  stbi_image_free(image);

  /** Set index count */
  index_count = indices.size();
}

void Terrain::setGraphicsData() {
  /** Create VAO / VBO / EBO */
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(va_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
  glVertexAttribPointer(va_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normals));
  glVertexAttribPointer(va_texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoord));

  glEnableVertexAttribArray(va_position);
  glEnableVertexAttribArray(va_normal);
  glEnableVertexAttribArray(va_texcoord);

  GFX_INFO("Generated terrain of (%u vertices).", vertices.size());
}

void Terrain::draw(Shader shader) {
  auto model = glm::mat4(1.0f);
  auto normal_matrix = glm::mat3(model);

  shader.setMat4("model", model);
  shader.setMat3("normal_matrix", normal_matrix);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBindVertexArray(vao);

  texture.bind(tb_diffuse);
  glDrawElements(GL_TRIANGLE_STRIP, index_count, GL_UNSIGNED_INT, nullptr);

  // set everything to default
  glBindVertexArray(0);
  glActiveTexture(GL_TEXTURE0);
}

glm::vec3 Terrain::calculateNormal(int x, int z, unsigned char* image) {
  int index = (x - 1 + (z * 1080)) * 3;
  float heightL = calculateHeight((int)image[index]);
  
  index = (x + 1 + (z * 1080)) * 3;
  float heightR = calculateHeight((int)image[index]);
  
  index = (x + ((z - 1) * 1080)) * 3;
  float heightD = calculateHeight((int)image[index]);
  
  index = (x  + ((z + 1) * 1080)) * 3;
  float heightU = calculateHeight((int)image[index]);

  glm::vec3 normal = glm::vec3(heightL - heightR, 2.0f, heightD - heightU);
  normal = glm::normalize(normal);
  return normal;
}

float Terrain::calculateHeight(int height) {
  float result = height / 255.0f;
  height -= 0.5;
  result *= 60;
  return result;
}