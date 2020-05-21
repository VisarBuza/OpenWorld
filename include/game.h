#pragma once
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "text_renderer.h"
#include "route_planner.h"
#include "model.h"
#include "camera.h"
#include "terrain.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_PAUSED,
    GAME_WIN,
    GAME_LOSS
};

enum CameraState {
  FIRST_PERSON,
  THIRD_PERSON,
  BIRD_VIEW
};

struct Eagle {
  glm::vec3 position{};
  glm::vec3 front = glm::vec3(0.0, 0.0, 1.0);
  glm::vec3 direction{};
  float speed;
  float scale = 3;
  float rotation = 0;
  float angle = 0;
  float radius = 200;
  float fluctuation = 5;
  float altitude = 20;
  Model model;
  void draw(Shader shader) {
    model.draw(position, scale, glm::degrees(rotation), glm::vec3(0.05, 1.0f, 0.0f), shader);
  }
  void update(float dt) {
    angle += dt;
    float newX = sin(angle / 3) * radius;
    float newY = sin(angle / 3) * 10;
    float newZ = cos(angle / 3) * radius;
    direction = glm::normalize(glm::vec3(newX, 0, newZ) - glm::vec3(position.x, 0, position.z));
    rotation += glm::acos(glm::dot(front, direction));
    front = direction;
    position.x = newX;
    position.y = altitude + newY;
    position.z = newZ;
  }
};

class Game {
  public:
    bool keys[1024];
    unsigned int score;
    float flashlight = false;
    GameState state;
    RoutePlanner route;
    Camera camera = Camera(glm::vec3(0.0f, 10.0f, 0.0f));

    void init();
    void update(float dt);
    void processInput(float dt);
    void render();
    void renderEndScreen();
  private:
    int view = 0;
    bool free = false;
    float angle = 0;
    Model lamp;
    Model tree;
    Model player;
    Model duck;
    Model deer;
    Eagle eagle;
    Model moose;
    Terrain terrain;
    
    Shader shader;
    Shader skyboxShader;
    TextRenderer text;
    
    void displayScore();
    void setLighting();
    void setUpTransformations();
    void loadModels();
    void loadTerrain();
    void loadShaders();
    void checkCollision(float dt);
    void updateDeers(float dt);
    void drawPlayer();
    void drawTrees();
    void drawLamps();
    void drawDucks();
    void drawDeers();
    void setDeers();
    void setDucks();
    void setTrees();
    void setLamps();
    std::vector<glm::vec3> treePos;
    std::vector<glm::vec3> lampPos;
    std::vector<glm::vec3> duckPos;
    std::vector<glm::vec3> deerPos;
};

#endif