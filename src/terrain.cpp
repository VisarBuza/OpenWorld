#include "terrain.h"
#include <gfx.h>
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

void Terrain::load(const char *file) {
  int width, height, channels;
  unsigned char *image = stbi_load(file, &width, &height, &channels, STBI_rgb);

  int xLength = 1080;
  int yLength = 1080;

  int offset = 0;
  for (int y = 0; y < yLength; y++) {
    for (int x = 0; x < xLength; x++) {
      float xRatio = x / (float)(xLength - 1);
      float yRatio = 1.0f - (y / (float)(yLength - 1));
      int index = (x + (y * yLength)) * 3;
      height = calculateHeight((int)image[index]);
      // Build our heightmap from the top down, so that our triangles are
      // counter-clockwise.
      float xPosition = -100.0 + (xRatio * 200);
      float yPosition = -100.0 + (yRatio * 200);

      vertices.push_back({});
      auto &vertex = vertices.back();
      vertex.position = glm::vec3(x, height, y);
      vertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
      vertex.texcoord = glm::vec2(0.0f);
    }
  }

  int numStripsRequired = yLength - 1;
  int numDegensRequired = 2 * (numStripsRequired - 1);
  int verticesPerStrip = 2 * xLength;

  for (int y = 0; y < yLength - 1; y++) {
    if (y > 0) {
      indices.push_back(y * yLength);
    }
    for (int x = 0; x < xLength; x++) {
      indices.push_back((y * yLength) + x);
      indices.push_back(((y + 1) * yLength) + x);
    }
    if (y < yLength - 2) {
      indices.push_back(((y + 1) * yLength) + (xLength - 1));
    }
  }

  stbi_image_free(image);

  /** Set index count */
  index_count = indices.size();

  /** Create VAO / VBO / EBO */
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normals));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoord));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
}

void Terrain::draw(Shader shader) {
  auto model = glm::mat4(1.0f);
  auto normal_matrix = glm::mat3(model);

  shader.setMat4("model", model);
  shader.setMat3("normal_matrix", normal_matrix);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBindVertexArray(vao);

  glDrawElements(GL_TRIANGLE_STRIP, index_count, GL_UNSIGNED_INT, nullptr);

  // set everything to default
  glBindVertexArray(0);
  glActiveTexture(GL_TEXTURE0);
}

float Terrain::calculateHeight(int height) {
  float result = height / 255.0f;
  result -= -0.5;
  result *= -40;
  return result;
}