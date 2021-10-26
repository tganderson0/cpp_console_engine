#include "GameObject.hpp"
#include "Input.hpp"
#include "Physics.hpp"
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
      std::make_shared<Physics>(gameObjects, 4);

  // Create player controller
  std::shared_ptr<Input> playerInput =
      std::make_shared<Input>(std::ref((gameObjects)->at(0)));

  // Start threads
  std::thread physics(&Physics::update, physicsController, std::ref(running));

  std::thread input(&Input::getInput, playerInput, std::ref(running));
  std::thread renderer(render, gameObjects, std::ref(running));

  input.join();
  renderer.join();
  physics.join();

  return 0;
}

/*
 * Thread that renders the map using the vector of GameObjects
 * @param gameObjects The shared pointer containing the list of gameObjects
 * @param running The boolean that controls if the program should be currently
 * running
 */
void render(std::shared_ptr<std::vector<GameObject>> gameObjects,
            bool &running) {
  // To start off, we should create an empty map
  std::vector<std::string> map;
  for (decltype(rlutil::trows()) i = 0; i < rlutil::trows(); i++) {
    map.push_back(
        std::string(rlutil::tcols() - 1, ' ')); // Add an empty map to start
  }
  // Main loop
  while (running) {
    // Updating Map
    std::vector<std::string> old_map =
        map; // copy the previous state of the map

    // Reset the map
    map.clear();
    for (decltype(rlutil::trows()) i = 0; i < rlutil::trows(); i++) {
      map.push_back(
          std::string(rlutil::tcols() - 1, ' ')); // Add an empty map to start
    }

    // Update the map

    for (auto &gameObject : *gameObjects) {
      int x = static_cast<int>(gameObject.getX());
      int y = static_cast<int>(gameObject.getY());
      for (int row = 0; row < gameObject.getHeight(); row++) {
        for (int col = 0; col < gameObject.getWidth(); col++) {
          if (x + col >= 0 && x + col < rlutil::tcols() - 1 && y + row >= 0 &&
              y + row <
                  rlutil::trows() -
                      1) { // make sure the object is in the valid render space
            map.at(y + row).at(x + col) =
                gameObject.repr; // Change it to the new state
          }
        }
      }
    }

    // Update only in the changed locations, to make it less laggy
    for (decltype(map.size()) row = 0; row < map.size(); row++) {
      for (decltype(map.size()) col = 0; col < map.at(row).size(); col++) {
        if (map.at(row).at(col) != old_map.at(row).at(col)) {
          rlutil::locate(col + 1, row + 1);
          rlutil::setChar(map.at(row).at(col));
        }
      }
    }
  }
}