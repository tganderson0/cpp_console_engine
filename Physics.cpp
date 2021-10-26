#include "Physics.hpp"
#include "GameObject.hpp"
#include "rlutil.h"
#include <thread>

Physics::Physics(std::shared_ptr<std::vector<GameObject>> gameObjects,
                 int gravity)
    : m_gameObjects(gameObjects), m_gravity(gravity),
      m_lastTime(std::chrono::steady_clock::now()) {}

void Physics::update(bool &running) {
  while (running) {
    auto newPoint = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(newPoint -
                                                                 m_lastTime)
               .count() < refreshRateMs) {
      auto temp = std::chrono::duration_cast<std::chrono::milliseconds>(
                      newPoint - m_lastTime)
                      .count();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      newPoint = std::chrono::steady_clock::now();
    }
    auto deltaTime = static_cast<double>(
                         std::chrono::duration_cast<std::chrono::microseconds>(
                             newPoint - m_lastTime)
                             .count()) /
                     1000000;
    m_lastTime = newPoint;

    for (GameObject &gameObject : *m_gameObjects) {

      if (gameObject.gravityAffects) { // Only update this one if gravity can
                                       // affect it

        // Update y position and velocity
        gameObject.setY(gameObject.getY() -
                        ((gameObject.velocityY * deltaTime) -
                         (0.5 * (m_gravity - gameObject.accelerationY) *
                          pow(deltaTime, 2))));
        gameObject.velocityY = gameObject.velocityY - (deltaTime * m_gravity);
        ; // Convert back to seconds

        // Update x position and velocity
        gameObject.setX(gameObject.getX() + (gameObject.velocityX * deltaTime));
        gameObject.velocityX =
            gameObject.velocityX + (gameObject.accelerationX * deltaTime);
      }

      // So we don't fall through the bottom of the screen
      if (gameObject.getY() >= rlutil::trows() - 2) {
        gameObject.velocityY = 0;
        gameObject.velocityX = 0;
        gameObject.setY(rlutil::trows() - 2);
      }
    }
  }
}