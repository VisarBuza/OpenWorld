#include "texture.h"

#include <algorithm>
#include <iostream>

#include <gfx.h>
#include <glad/glad.h>
#include <stb_image.h>

Texture::~Texture() noexcept { glDeleteTextures(1, &m_handle); }

Texture::Texture(Texture&& other) noexcept : m_handle(std::move(other.m_handle)) {}

Texture& Texture::operator=(Texture&& other) noexcept {
  if (this == &other) {
    return *this;
  }

  /** Clean and steal */
  glDeleteTextures(1, &m_handle);
  m_handle = std::move(other.m_handle);
  other.m_handle = 0u;

  return *this;
}

void Texture::load_texture(const std::string& filepath) {
  /** Clean up */
  glDeleteTextures(1, &m_handle);

  /** Create */
  uint32_t handle{};
  glGenTextures(1, &handle);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, handle);

  /** Parameters */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  /** Load */
  int x, y;
  stbi_set_flip_vertically_on_load(true);
  auto* pixels = stbi_load(filepath.c_str(), &x, &y, nullptr, STBI_rgb_alpha);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  glGenerateMipmap(GL_TEXTURE_2D);

  /** Finalize */
  stbi_image_free(pixels);
  GFX_INFO("Loaded image (%dx%d) from %s.", x, y, filepath.c_str());
  m_handle = handle;
}

void Texture::bind(uint32_t binding) {
  glActiveTexture(GL_TEXTURE0 + binding);
  glBindTexture(GL_TEXTURE_2D, m_handle);
}

Texture::operator uint32_t() { return m_handle; }

unsigned int Texture::loadCubeMap(std::vector<std::string> faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}
