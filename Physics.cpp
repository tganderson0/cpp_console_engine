#include "Physics.hpp"
#include "GameObject.hpp"
#include "rlutil.h"
#include <thread>

Physics::Physics(std::shared_ptr<std::vector<GameObject>> gameObjects,
                 int gravity)
    : m_gameObjects(gameObjects), m_gravity(gravity),
      m_lastTime(std::chrono::steady_clock::now()) {}

/*
 * Performs an update on all gameObjects. Uses the constant set in the header
 * file for refresh rate. Set to 0 for fastest refresh
 * @param running The master control for if the program should be running
 */
void Physics::update(bool &running) {
  while (running) {
    auto newPoint = std::chrono::steady_clock::now();

    // Waits for the elapsed time (deltaTime) to be greater than the specified
    // refreshRateMs
    while (std::chrono::duration_cast<std::chrono::milliseconds>(newPoint -
                                                                 m_lastTime)
               .count() < refreshRateMs) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      newPoint = std::chrono::steady_clock::now();
    }

    // Get the time elapsed since last call (should be close to refreshRateMs)
    auto deltaTime = static_cast<float>(
                         std::chrono::duration_cast<std::chrono::microseconds>(
                             newPoint - m_lastTime)
                             .count()) /
                     1000000;
    m_lastTime = newPoint;

    // Perform updates on all gameObjects that are affected by gravity (need to
    // add a "static" and "dynamic" type for this)
    for (GameObject &gameObject : *m_gameObjects) {

      if (gameObject.gravityAffects) { // Only update this one if gravity can
                                       // affect it

        // Update y position and velocity
        gameObject.setY(gameObject.getY() -
                        ((gameObject.velocityY * deltaTime) -
                         (0.5f * (m_gravity - gameObject.accelerationY) *
                          powf(deltaTime, 2))));
        gameObject.velocityY = gameObject.velocityY - (deltaTime * m_gravity);
        ;

        // Update x position and velocity
        gameObject.setX(gameObject.getX() + (gameObject.velocityX * deltaTime));
        gameObject.velocityX =
            gameObject.velocityX + (gameObject.accelerationX * deltaTime);
      }

      // So we don't fall through the bottom of the screen
      if (gameObject.getY() + gameObject.getHeight() >= rlutil::trows() - 2) {
        gameObject.velocityY = 0.0f;
        gameObject.setY(rlutil::trows() - 2.0f - gameObject.getHeight());
      }
    }
  }
}