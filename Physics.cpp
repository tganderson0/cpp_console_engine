#include "Physics.hpp"
#include "GameObject.hpp"
#include "rlutil.h"
#include <math.h>
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
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      newPoint = std::chrono::steady_clock::now();
    }

    // Get the time elapsed since last call (should be close to refreshRateMs)
    auto deltaTime = static_cast<float>(
                         std::chrono::duration_cast<std::chrono::microseconds>(
                             newPoint - m_lastTime)
                             .count()) /
                     1000000;
    deltaTime *= timeRate;
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

        // Update x position and velocity
        gameObject.setX(gameObject.getX() + (gameObject.velocityX * deltaTime));
        gameObject.velocityX =
            gameObject.velocityX + (gameObject.accelerationX * deltaTime);
        validPlacement(gameObject);
      }
    }
  }
}

/*
 * Checks if the move was valid. Fixes positioning if it was not
 * @param gameObject The object that is being checked
 */
void Physics::validPlacement(GameObject &gameObject) {
  for (GameObject &other : *m_gameObjects) {
    if (other.tag == gameObject.tag && gameObject.tag != "none") {
      continue;
    }

    /*
     * A note on all these casts to integers, this is because the console is
     * only whole numbers. While the 'real' position of the objects can be
     * floats the collisions are done based on what a player would see.
     */
    int myTop = static_cast<int>(gameObject.getY());
    int myBottom = static_cast<int>(myTop + gameObject.getHeight() - 1);
    int myLeft = static_cast<int>(gameObject.getX());
    int myRight = static_cast<int>(myLeft + gameObject.getWidth() - 1);

    int oTop = static_cast<int>(other.getY());
    int oBottom = static_cast<int>(oTop + other.getHeight() - 1);
    int oLeft = static_cast<int>(other.getX());
    int oRight = static_cast<int>(oLeft + other.getWidth() - 1);

    // First we should check if we are completely contained

    if ((myTop >= oTop && myTop <= oBottom) ||
        (myBottom <= oBottom && myBottom >= oTop)) {

      if ((myRight <= oRight) &&
          (myLeft >= oLeft)) { // We are completely contained, so we should just
                               // move up or down
        bool onOrUnder = ((myRight >= oLeft && myRight <= oRight) ||
                          (myLeft <= oRight && myLeft >= oLeft));
        if (gameObject.velocityY < 0 && onOrUnder) { // Collision falling down
          gameObject.setY(static_cast<float>(oTop - gameObject.getHeight()));
          gameObject.velocityY = 0;
        } else if (gameObject.velocityY > 0 &&
                   onOrUnder) { // Collision going upwards
          gameObject.setY(static_cast<float>(oBottom + 1));
          gameObject.velocityY = 0;
        }
      } else if ((myBottom <= oBottom && myTop >= oTop) &&
                     ((myLeft >= oLeft && myLeft <= oRight)) ||
                 (myRight >= oLeft &&
                  myRight <= oRight)) { // Complete side collision, no top
        if (myRight < oRight) {         // Collision gameObject going right
          gameObject.setX(static_cast<float>(oLeft - gameObject.getWidth()));
          gameObject.velocityX = 0;
        } else if (myLeft > oLeft) { // Collision gameObject going left
          gameObject.setX(static_cast<float>(oRight + 1));
          gameObject.velocityX = 0;
        }
      } else if ((myRight >= oLeft && myRight <= oRight) ||
                 (myLeft <= oRight && myLeft >= oLeft)) {
        // Handling Horizontal Collisions
        if (gameObject.velocityX > 0) { // Collision gameObject going right
          gameObject.setX(static_cast<float>(oLeft - gameObject.getWidth()));
          gameObject.velocityX = 0;
        } else if (gameObject.velocityX <
                   0) { // Collision gameObject going left
          gameObject.setX(static_cast<float>(oRight + 1));
          gameObject.velocityX = 0;
        }
        myLeft = static_cast<int>(gameObject.getX());
        myRight = static_cast<int>(myLeft + gameObject.getWidth() - 1);
        bool onOrUnder = ((myRight >= oLeft && myRight <= oRight) ||
                          (myLeft <= oRight && myLeft >= oLeft));
        if (gameObject.velocityY < 0 && onOrUnder) { // Collision falling down
          gameObject.setY(static_cast<float>(oTop - gameObject.getHeight()));
          gameObject.velocityY = 0;
        } else if (gameObject.velocityY > 0 &&
                   onOrUnder) { // Collision going upwards
          gameObject.setY(static_cast<float>(oBottom + 1));
          gameObject.velocityY = 0;
        }
      }
    }
  }
}