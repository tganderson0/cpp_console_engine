#include "GameObject.hpp"
#include "Input.hpp"
#include "Physics.hpp"
#include "Renderer.hpp"
#include "rlutil.h"
#include <memory>
#include <string>
#include <thread>
#include <vector>

int main() {

  rlutil::hidecursor();
  rlutil::cls();

  // Create the GameObjects pointer
  std::shared_ptr<std::vector<GameObject>> gameObjects =
      std::make_shared<std::vector<GameObject>>();
  *(gameObjects) = std::vector<GameObject>(0);

  // The boolean that controls it all
  bool running = true;

  // Adding in a player
  GameObject player(5.0f, 5.0f, 'o', 3, 3);
  (gameObjects)->push_back(player);

  // Adding in player 2
  GameObject player2(10.0f, 10.0f, 'x', 3, 3);
  (gameObjects)->push_back(player2);

  // Create physics controller
  std::shared_ptr<Physics> physicsController =
      std::make_shared<Physics>(gameObjects, 9.8f);

  // Create player controller
  std::shared_ptr<Input> playerInput =
      std::make_shared<Input>(std::ref((gameObjects)->at(0)));

  // Create player controller2
  std::shared_ptr<Input> playerInput2 =
      std::make_shared<Input>(std::ref((gameObjects)->at(1)));

  // Create the renderer
  std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(gameObjects);

  // Start threads
  std::thread physicsThread(&Physics::update, physicsController,
                            std::ref(running));

  // More cross platform (but not as good)
  // std::thread inputThread(&Input::getInput, playerInput, std::ref(running));
  // Windows only (better)
  std::thread inputThread(&Input::windowsInput, playerInput, std::ref(running));
  std::thread inputThread2(&Input::windowsInput2, playerInput2,
                           std::ref(running));
  std::thread renderThread(&Renderer::render, renderer, std::ref(running));

  // Wait for completion
  inputThread.join();
  inputThread2.join();
  renderThread.join();
  physicsThread.join();

  rlutil::showcursor();

  return 0;
}