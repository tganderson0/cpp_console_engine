#include "GameObject.hpp"

// repr is the character representation of the gameobject
GameObject::GameObject(int x, int y, char repr)
    : m_x(x), m_y(y), m_repr(repr) {}