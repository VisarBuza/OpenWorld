#include "game.h"
#include <stdlib.h>

void Game::init() {
  state = GAME_MENU;
  loadShaders();
  loadModels();
  loadTerrain();
  setTrees();
  setLamps();
  setDucks();
  setDeers();
  score = 0;
}

void Game::update(float dt) {
  terrain.update(dt);
  if (!free) {
    camera.Position.y = terrain.getHeight(camera.Position.x, camera.Position.z) + 2;
  }
  eagle.update(dt);
  // updateDeers(dt);
}

void Game::processInput(float dt) {
  if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, dt);
  if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, dt);
  if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, dt);
  if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, dt);
  if (keys[GLFW_KEY_F]) flashlight = !flashlight;
  if (keys[GLFW_KEY_M]) view = (view + 1) % 3;
  if (keys[GLFW_KEY_T]) free = !free;
  if (keys[GLFW_KEY_LEFT_SHIFT]) {
    camera.ProcessKeyboard(RUN, dt);
  } else {
    camera.ProcessKeyboard(STOP_RUN, dt);
  }
}

void Game::render() {
  shader.reset();
  setLighting();
  setUpTransformations();
  terrain.draw(shader);
  if (view != FIRST_PERSON) {
    player.draw(
        glm::vec3(camera.Position.x, terrain.getHeight(camera.Position.x, camera.Position.z), camera.Position.z), 0.15,
        0, glm::vec3(1.0, 0.0, 0.0), shader);
  }
  eagle.draw(shader);
  moose.draw(glm::vec3(300, terrain.getHeight(300, 200), 200), 1, -90, glm::vec3(1.0, 0.0, 0.0), shader);
  drawDucks();
  drawDeers();
  drawTrees();
  drawLamps();
  auto projection = glm::perspective(camera.zoom, 16.f / 9.f, 0.01f, 650.f);
  terrain.drawSkybox(skyboxShader, camera.GetFirstPersonView(), projection);
}

void Game::renderEndScreen() {
  std::stringstream ss;
  ss << score;

  if (state == GAME_WIN) {
    text.renderText("You win!", 320.0, Config::SCR_HEIGHT / 2 - 20.0, 1.0);
  } else {
    text.renderText("You lose!", 320.0, Config::SCR_HEIGHT / 2 - 20.0, 1.0);
  }

  text.renderText("Score:" + ss.str(), 325.0, Config::SCR_HEIGHT / 2, 1.0);
}

void Game::setUpTransformations() {
  auto projection = glm::perspective(camera.zoom, 16.f / 9.f, 0.01f, 650.f);
  auto lookAt = glm::mat4(1.0f);

  switch (view) {
    case FIRST_PERSON:
      lookAt = camera.GetFirstPersonView();
      break;
    case THIRD_PERSON:
      lookAt = camera.GetThirdPersonView();
      break;
    case BIRD_VIEW:
      lookAt = camera.Get2DView();
      break;
  }

  shader.setMat4("view", lookAt);
  shader.setMat4("projection", projection);
}

void Game::setLighting() {
  shader.setDirLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.0f), glm::vec3(.25f), glm::vec3(0.5f));
  shader.setSpotLight(camera.Position, camera.Front);
  shader.toggleFlashlight(flashlight);
}

void Game::displayScore() {
  std::stringstream ss;
  ss << this->score;
  text.renderText("Score: " + ss.str(), 5.0f, 5.0f, 1.0);
}

void Game::checkCollision(float dt) {
  
}

void Game::loadModels() {
  lamp.load("resources/assets/Objects/Lamp/lamp.obj", "resources/assets/Objects/Lamp/lamp.png");
  tree.load("resources/assets/Objects/PineTree2/10447_Pine_Tree_v1_L3b.obj",
            "resources/assets/Objects/PineTree2/10447_Pine_Tree_v1_Diffuse.jpg");
  player.load("resources/assets/Objects/Player/person.obj", "resources/assets/Objects/Player/playerTexture.png");
  eagle.model.load("resources/assets/Objects/Eagle/Eagle02.obj", "resources/assets/Objects/Eagle/Eagle02.png");
  eagle.position = glm::vec3(0, 20, 0);
  eagle.scale = 3.0;
  moose.load("resources/assets/Objects/MooseFemale/12959_Moose_Female_v1_l3.obj",
             "resources/assets/Objects/MooseFemale/12959_Moose_Female_diff.jpg");
  deer.load("resources/assets/Objects/Deer1/12961_White-Tailed_Deer_v1_l2.obj",
             "resources/assets/Objects/Deer1/12961_White-TailedDeer_diffuse.jpg");
  duck.load("resources/assets/Objects/Duck/12248_Bird_v1_L2.obj",
            "resources/assets/Objects/Duck/12248_Bird_v1_diff.jpg");
}

void Game::loadTerrain() {
  terrain.load("resources/assets/HeightMaps/Trondheim_HeightMap.png", "resources/assets/Textures/terrain.jpeg");
  terrain.loadSkybox();
}

void Game::loadShaders() {
  shader = Shader("../resources/shaders/vertex.vert", "../resources/shaders/fragment.frag");
  skyboxShader = Shader("../resources/shaders/skybox/skybox.vert", "../resources/shaders/skybox/skybox.frag");
  text = TextRenderer(Config::SCR_WIDTH, Config::SCR_HEIGHT);
  text.load("resources/fonts/ocraext.TTF", 24);
}

void Game::setTrees() {
  int z = -400;
  int x = -400;
  while (z < 400) {
    x = -400;
    while (x < 400) {
      float height = terrain.getHeight(x, z);
      if (height > -35 && height < -10) {
        treePos.push_back(glm::vec3(x, height, z));
      }
      x += 30;
    }
    z += 30;
  }
}

void Game::setLamps() {
  int z = -500;
  int x = -500;
  int index = 0;
  while (z < 500) {
    x = -500;
    while (x < 500) {
      float height = terrain.getHeight(x, z);
      if (height > -35 && height < -5) {
        lampPos.push_back(glm::vec3(x, height, z));
      }
      x += 100;
    }
    z += 100;
  }
}

void Game::setDucks() {
  int z = -400;
  int x = -400;
  int index = 0;
  while (z < 400) {
    x = -400;
    while (x < 400) {
      float height = terrain.getHeight(x, z);
      if (height <= -36) {
        duckPos.push_back(glm::vec3(x, height - 0.2, z));
      }
      x += 60;
    }
    z += 60;
  }
}

void Game::setDeers() {
  int z = -100;
  int x = -100;
  int index = 0;
  while (z < 100) {
    x = -100;
    while (x < 100) {
      float height = terrain.getHeight(x, z);
      if (height > -35 && height < -5) {
        deerPos.push_back(glm::vec3(x, height - 0.2, z));
      }
      x += 40;
    }
    z += 40;
  }
}

void Game::drawTrees() {
  for (auto &pos : treePos) {
    tree.draw(pos, 0.02, -90, glm::vec3(1.0, 0.0, 0.0), shader);
  }
}

void Game::drawDeers() {
  for (auto &pos : deerPos) {
    deer.draw(pos, 0.04, -90, glm::vec3(1.0, 0.0, 0.0), shader);
  }
}

void Game::drawDucks() {
  for (auto &pos : duckPos) {
    duck.draw(pos, 0.02, -90, glm::vec3(1.0, 0.0, 0.0), shader);
  }
}

void Game::drawLamps() {
  int index = 0;
  for (auto &pos : lampPos) {
    lamp.draw(pos, 0.3, 0, glm::vec3(1.0, 0.0, 0.0), shader);
    if (index < 10) {
      shader.setPointLights(glm::vec3(pos.x, pos.y + 0.5, pos.z), glm::vec3(index / 10, 1 - (index / 10), 0.0),
                            index++);
    }
    if (index == 10) {
      break;
    }
  }
}

void Game::updateDeers(float dt) {
  angle += dt;
  for (auto &pos : deerPos) {
    pos.x += cos(angle / 20) * 100;
    pos.z += sin(angle / 20) * 100;
    pos.y = terrain.getHeight(pos.x, pos.z);
  }
}