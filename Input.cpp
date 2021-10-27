#include "Input.hpp"

Input::Input(GameObject &player) : m_player(player) {}

const float horizontalSpeed = 15.0f;
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
        m_player.velocityX = -1 * horizontalSpeed;
      }
    }
    if (input == rlutil::KEY_RIGHT) {
      if (m_player.velocityX < 0) {
        m_player.velocityX = 0.0f;
      } else {
        m_player.velocityX = horizontalSpeed;
      }
    }
    if (input == rlutil::KEY_SPACE) {
      m_player.velocityY = 10.0f;
    }
  }
}

/*
 * The thread that is in change of taking input
 * @param running The boolean that is updated when the game should stop running
 */
void Input::windowsInput(bool &running) {

  while (running) {

    // Escape key has been toggled
    if (GetKeyState(VK_ESCAPE) & 0x8000) {
      running = false;
      return; // exits game
    }
    // Move character

    // Jumping
    if (GetKeyState(VK_UP) & 0x8000) {
      m_player.velocityY = 10.0f;
    }
    if (GetKeyState(VK_SPACE) & 0x8000) {
      m_player.velocityY = 10.0f;
    }

    // Moving
    if ((GetKeyState(VK_LEFT) & 0x8000) && (GetKeyState(VK_RIGHT) & 0x8000)) {
      m_player.velocityX = 0.0f;
    } else if (GetKeyState(VK_LEFT) & 0x8000) {
      m_player.velocityX = -10.0f;
    } else if (GetKeyState(VK_RIGHT) & 0x8000) {
      m_player.velocityX = 10.0f;
    } else {
      m_player.velocityX = 0.0f;
    }
  }
}

/*
 * The thread that is in change of taking input for player 2
 * @param running The boolean that is updated when the game should stop running
 */
void Input::windowsInput2(bool &running) {

  while (running) {

    // Move character

    // Jumping
    if (GetKeyState('W') & 0x8000) {
      m_player.velocityY = 10.0f;
    }

    // Moving
    if ((GetKeyState('A') & 0x8000) && (GetKeyState('D') & 0x8000)) {
      m_player.velocityX = 0.0f;
    } else if (GetKeyState('A') & 0x8000) {
      m_player.velocityX = -10.0f;
    } else if (GetKeyState('D') & 0x8000) {
      m_player.velocityX = 10.0f;
    } else {
      m_player.velocityX = 0.0f;
    }
  }
}

/*
 * Reference for virtual key codes:
 * https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
 * 0x8000 is for activated
 *
 * Reference for how to use:
 * https://stackoverflow.com/questions/41600981/how-do-i-check-if-a-key-is-pressed-on-c
 */