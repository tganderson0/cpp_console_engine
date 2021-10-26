#include "GameObject.hpp"
#include "rlutil.h"
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

std::mutex playerMutex;

void input_thread(GameObject &player, bool &running);
void render(std::shared_ptr<std::vector<GameObject>> gameObjects,
            bool &running);

int main() {
  rlutil::hidecursor();

  std::shared_ptr<std::vector<GameObject>> gameObjects =
      std::make_shared<std::vector<GameObject>>();
  bool running = true;
  (gameObjects)->push_back(GameObject(5, 5, 'o'));

  // Start input thread
  std::thread input(input_thread, std::ref(gameObjects.get()->at(0)),
                    std::ref(running));
  std::thread renderer(render, gameObjects, std::ref(running));

  input.join();
  renderer.join();

  return 0;
}

void input_thread(GameObject &player, bool &running) {

  while (running) {
    int input = rlutil::getkey();

    if (input == rlutil::KEY_ESCAPE) {
      std::cout << "leaving game" << std::endl;
      running = false;
      return; // exits game
    }

    rlutil::locate(1, 1);
    std::cout << "Pushed: " << input;
    // Move character

    playerMutex.lock(); // Wait until the player is not being modified
    if (input == rlutil::KEY_UP) {
      player.m_y -= 1;
    }
    if (input == rlutil::KEY_DOWN) {
      player.m_y += 1;
    }
    if (input == rlutil::KEY_LEFT) {
      player.m_x -= 1;
    }
    if (input == rlutil::KEY_RIGHT) {
      player.m_x += 1;
    }

    // Perform checks
    int screenX = rlutil::tcols();
    int screenY = rlutil::trows();
    if (player.m_x < 0) {
      player.m_x = 0;
    }
    if (player.m_x > screenX - 2) {
      player.m_x = screenX - 2;
    }
    if (player.m_y < 0) {
      player.m_y = 0;
    }
    if (player.m_y > screenY - 1) {
      player.m_y = screenY - 1;
    }
    playerMutex.unlock(); // We are done modifying the player
  }
}

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

    playerMutex.lock(); // We are updating the player, this should wait
    // Update the map
    for (auto gameObject : *gameObjects) {
      map.at(gameObject.m_y).at(gameObject.m_x) =
          gameObject.m_repr; // Change it to the new state
      playerMutex.unlock();  // Unlock after the first
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