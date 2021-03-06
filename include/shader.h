#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

/** Constants for uniform locations in vertex shader */
constexpr auto ul_mat_model = 0;
constexpr auto ul_mat_view = 1;
constexpr auto ul_mat_proj = 2;
constexpr auto ul_mat_normal = 3;

/** Constants for uniform locations in fragment shader */
constexpr auto ul_vec_light_pos = 4;
constexpr auto ul_vec_spot_dir = 5;
constexpr auto ul_f_innerCutoff = 6;
constexpr auto ul_f_outerCutoff = 7;
constexpr auto ul_vec_view_pos = 8;

class Shader {
 public:
  unsigned int ID;
  // constructor generates the shader on the fly
  // ------------------------------------------------------------------------
  Shader() {}

  Shader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
      // open files
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      std::stringstream vShaderStream, fShaderStream;
      // read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      // close file handlers
      vShaderFile.close();
      fShaderFile.close();
      // convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
      // if geometry shader path is present, also load a geometry shader
      if (geometryPath != nullptr) {
        gShaderFile.open(geometryPath);
        std::stringstream gShaderStream;
        gShaderStream << gShaderFile.rdbuf();
        gShaderFile.close();
        geometryCode = gShaderStream.str();
      }
    } catch (std::ifstream::failure &e) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // if geometry shader is given, compile geometry shader
    unsigned int geometry;
    if (geometryPath != nullptr) {
      const char *gShaderCode = geometryCode.c_str();
      geometry = glCreateShader(GL_GEOMETRY_SHADER);
      glShaderSource(geometry, 1, &gShaderCode, NULL);
      glCompileShader(geometry);
      checkCompileErrors(geometry, "GEOMETRY");
    }
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if (geometryPath != nullptr) glAttachShader(ID, geometry);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr) glDeleteShader(geometry);
  }
  // activate the shader
  // ------------------------------------------------------------------------
  void use() { glUseProgram(ID); }

  // utility uniform functions
  // static void setModelMat(glm::mat4 model) { glUniformMatrix4fv(ul_mat_model, 1, GL_FALSE, glm::value_ptr(model)); }

  // static void setViewMat(glm::mat4 view) { glUniformMatrix4fv(ul_mat_view, 1, GL_FALSE, glm::value_ptr(view)); }

  // static void setProjMat(glm::mat4 proj) { glUniformMatrix4fv(ul_mat_proj, 1, GL_FALSE, glm::value_ptr(proj)); }

  // static void setNormalMat(glm::mat3 normal) { glUniformMatrix3fv(ul_mat_normal, 1, GL_FALSE, glm::value_ptr(normal)); }

  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
  }
  // ------------------------------------------------------------------------
  void setInt(const std::string &name, int value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
  // ------------------------------------------------------------------------
  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }
  // ------------------------------------------------------------------------
  void setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
  }
  // ------------------------------------------------------------------------
  void setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
  }
  // ------------------------------------------------------------------------
  void setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
  }
  void setVec4(const std::string &name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
  }
  // ------------------------------------------------------------------------
  void setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  void setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }
  // ------------------------------------------------------------------------
  void setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
  }

  void setDirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    setVec3("dirLight.direction", direction);
    setVec3("dirLight.ambient", ambient);
    setVec3("dirLight.diffuse", diffuse);
    setVec3("dirLight.specular", specular);
  }

  void setSpotLight(glm::vec3 position, glm::vec3 direction) {
    setVec3("spotLight.position", position);
    setVec3("spotLight.direction", direction);
    setVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
    setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
    setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    setFloat("spotLight.constant", 1.0f);
    setFloat("spotLight.linear", 0.09);
    setFloat("spotLight.quadratic", 0.032);
    setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    setFloat("spotLight.outerCutOff", glm::cos(glm::radians(20.0f)));
  }

  void toggleFlashlight (bool state) {
    if (state) {
      setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
      setFloat("spotLight.outerCutOff", glm::cos(glm::radians(20.0f)));  
    } else {
      setFloat("spotLight.cutOff", glm::cos(glm::radians(0.0f)));
      setFloat("spotLight.outerCutOff", glm::cos(glm::radians(0.0f)));
    }
  }

  void reset() {
    this->use();
    glClearColor(0.06f, 0.06f, 0.06f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void setGameObjectLight(glm::vec3 position, glm::vec3 color, int index) {
    setVec3("ghostLights[" + std::to_string(index) + "].position", position);
    setVec3("ghostLights[" + std::to_string(index) + "].ambient", color * glm::vec3(0.1f));
    setVec3("ghostLights[" + std::to_string(index) + "].diffuse", color);
    setVec3("ghostLights[" + std::to_string(index) + "].specular", glm::vec3(0.8f));
    setFloat("ghostLights[" + std::to_string(index) + "].constant", 1.0f);
    setFloat("ghostLights[" + std::to_string(index) + "].linear", 0.7);
    setFloat("ghostLights[" + std::to_string(index) + "].quadratic", 1.4);
  }

  void setPointLights(glm::vec3 position, glm::vec3 color, int index) {
    setVec3("pointLights[" + std::to_string(index) + "].position", position);
    setVec3("pointLights[" + std::to_string(index) + "].ambient", color * glm::vec3(0.2f));
    setVec3("pointLights[" + std::to_string(index) + "].diffuse", color);
    setVec3("pointLights[" + std::to_string(index) + "].specular", glm::vec3(0.8f));
    setFloat("pointLights[" + std::to_string(index) + "].constant", 1.0f);
    setFloat("pointLights[" + std::to_string(index) + "].linear", 0.01);
    setFloat("pointLights[" + std::to_string(index) + "].quadratic", 0.002);
  }

 private:
  // utility function for checking shader compilation/linking errors.
  // ------------------------------------------------------------------------
  void checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    }
  }
};
#endif