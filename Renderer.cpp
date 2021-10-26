#include "Renderer.hpp"
#include "rlutil.h"

Renderer::Renderer(std::shared_ptr<std::vector<GameObject>> gameObjects)
    : m_gameObjects(gameObjects) {}

/*
 * Thread that renders the map using the vector of GameObjects
 * @param running The boolean that controls if the program should be currently
 * running
 */
void Renderer::render(bool &running) {
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

    for (auto &gameObject : *m_gameObjects) {
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