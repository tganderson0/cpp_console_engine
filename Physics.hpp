#pragma once
#include "GameObject.hpp"
#include <memory>
#include <vector>

class Physics {
public:
  Physics(std::shared_ptr<std::vector<GameObject>>, int = 10);
  void update(bool &running);

private:
  std::shared_ptr<std::vector<GameObject>> m_gameObjects;
  int m_gravity;
  std::chrono::steady_clock::time_point m_lastTime;
  const int refreshRateMs = 10;
};