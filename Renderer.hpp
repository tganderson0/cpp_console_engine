#pragma once
#include "GameObject.hpp"
#include <memory>
#include <vector>

class Renderer {
public:
  Renderer(std::shared_ptr<std::vector<GameObject>> gameObjects);
  void render(bool &running);

private:
  std::shared_ptr<std::vector<GameObject>> m_gameObjects;
};