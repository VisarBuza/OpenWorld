#include "terrain.h"
#include <gfx.h>
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

void Terrain::load(const char *file) {
  int width, height, channels;
  unsigned char *image = stbi_load(file, &width, &height, &channels, STBI_rgb);

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      vertices.push_back({});
      indices.push_back(indices.size());
      int index = (j + (i * width)) * 3;
      float height = calculateHeight((int)image[index]);

      auto &vertex = vertices.back();
      vertex.position = glm::vec3(j, height, i);
      vertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
      vertex.texcoord = glm::vec2(0.0f, 0.0f);
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

  glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr);
  
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