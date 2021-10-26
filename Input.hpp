#pragma once
#include "GameObject.hpp"
#include "rlutil.h"
#include <Windows.h>

class Input {
public:
  Input(GameObject &player);
  void getInput(bool &running);
  void windowsInput(bool &running);
  void windowsInput2(bool &running);

private:
  GameObject &m_player;
};
