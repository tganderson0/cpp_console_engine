#include "Input.hpp"

Input::Input(GameObject &player) : m_player(player) {}

/*
 * The thread that is in change of taking input
 * @param running The boolean that is updated when the game should stop running
 */
void Input::getInput(bool &running) {

  while (running) {
    int input = rlutil::getkey();

    if (input == rlutil::KEY_ESCAPE) {
      std::cout << "leaving game" << std::endl;
      running = false;
      return; // exits game
    }
    // Move character

    if (input == rlutil::KEY_UP) {
      m_player.velocityY = 10.0f;
    }
    if (input == rlutil::KEY_LEFT) {
      if (m_player.velocityX > 0) {
        m_player.velocityX = 0.0f;
      } else {
        m_player.velocityX = -10.0f;
      }
    }
    if (input == rlutil::KEY_RIGHT) {
      if (m_player.velocityX < 0) {
        m_player.velocityX = 0.0f;
      } else {
        m_player.velocityX = 10.0f;
      }
    }
    if (input == rlutil::KEY_SPACE) {
      m_player.velocityY = 10.0f;
    }
  }
}