#include "GameObject.hpp"
#include "Input.hpp"
#include "Physics.hpp"
#include "Renderer.hpp"
#include "rlutil.h"
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

void input_thread(GameObject &player, bool &running);
void render(std::shared_ptr<std::vector<GameObject>> gameObjects,
            bool &running);

int main() {

  rlutil::hidecursor();

  // Create the GameObjects pointer
  std::shared_ptr<std::vector<GameObject>> gameObjects =
      std::make_shared<std::vector<GameObject>>();
  *(gameObjects) = std::vector<GameObject>(0);

  // The boolean that controls it all
  bool running = true;

  // Adding in a player
  GameObject player(5, 5, 'o', 3, 3);
  (gameObjects)->push_back(player);

  // Create physics controller
  std::shared_ptr<Physics> physicsController =
      std::make_shared<Physics>(gameObjects, 9.8f);

  // Create player controller
  std::shared_ptr<Input> playerInput =
      std::make_shared<Input>(std::ref((gameObjects)->at(0)));

  // Create the renderer
  std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(gameObjects);

  // Start threads
  std::thread physicsThread(&Physics::update, physicsController,
                            std::ref(running));
  std::thread inputThread(&Input::getInput, playerInput, std::ref(running));
  std::thread renderThread(&Renderer::render, renderer, std::ref(running));

  inputThread.join();
  renderThread.join();
  physicsThread.join();

  return 0;
}