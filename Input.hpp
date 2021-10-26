#pragma once
#include "GameObject.hpp"
#include "rlutil.h"

class Input {
public:
  Input(GameObject &player);
  void getInput(bool &running);

private:
  GameObject &m_player;
};
