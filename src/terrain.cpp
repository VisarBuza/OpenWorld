#include "terrain.h"
#include <gfx.h>
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

std::vector<std::string> faces
{
  "resources/assets/Textures/skybox/right.png",
  "resources/assets/Textures/skybox/left.png",
  "resources/assets/Textures/skybox/top.png", 
  "resources/assets/Textures/skybox/bottom.png",
  "resources/assets/Textures/skybox/back.png",
  "resources/assets/Textures/skybox/front.png"
};

std::vector<std::string> facesNight
{
  "resources/assets/Textures/skybox/nightRight.png",
  "resources/assets/Textures/skybox/nightLeft.png",
  "resources/assets/Textures/skybox/nightTop.png",
  "resources/assets/Textures/skybox/nightBottom.png",
  "resources/assets/Textures/skybox/nightBack.png",
  "resources/assets/Textures/skybox/nightFront.png"
};

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
  for (int z = 0; z < height; z++) {
    for (int x = 0; x < width; x++) {
      int index = (x + (z * height)) * 3;
      float yHeight = calculateHeight((int)image[index]);
      

      vertices.push_back({});
      auto &vertex = vertices.back();
      vertex.position = glm::vec3(x - 540, yHeight, z - 540);
      vertex.normals = calculateNormal(x, z, image);
      vertex.texcoord = glm::vec2(((int)image[index])/ 255.0, 0);
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
  shader.use();
  shader.setVec3("skyColor", glm::vec3(0.5f));
  auto model = glm::mat4(1.0f);
  auto normal_matrix = glm::mat3(model);
  shader.setDirLight(direction, ambient, diffuse, specular);
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

void Terrain::loadSkybox() {
  cubemapTextureDay = texture.loadCubeMap(faces);
  cubeMapTextureNight = texture.loadCubeMap(facesNight);

  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Terrain::drawSkybox(Shader shader, glm::mat4 view, glm::mat4 projection) {
  glDepthMask(GL_FALSE);
  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
  shader.use();
  shader.setInt("skybox", 0);
  shader.setInt("skybox2", 1);
  shader.setVec3("fogColor", glm::vec3(0.5f));
  view = glm::mat4(glm::mat3(view)); // remove translation from the view matrix
  view = glm::rotate(view, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
  shader.setFloat("blendFactor", blendFactor);
  rotation += 0.03;
  if (rotation >= 360) rotation = 0;
  // skybox cube
  glBindVertexArray(skyboxVAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureDay);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureNight);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);
  glDepthMask(GL_TRUE);
}

void Terrain::update(float dt) {
  time += dt;
  blendFactor = glm::sin(time / 8) / 2 + 0.5;
  ambient = glm::vec3((1 - blendFactor) / 3, 0.1f, blendFactor / 2);
  diffuse = glm::vec3(1 - blendFactor);
  specular = glm::vec3(1 - blendFactor);
  direction = glm::vec3(0.0f, glm::sin(time / 8) , glm::cos(time / 8));
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
  if (result < 10) {
    result = 10;
  }
  return result;
}
