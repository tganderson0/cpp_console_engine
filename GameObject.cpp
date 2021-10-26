#include "GameObject.hpp"
#include <chrono>

// repr is the character representation of the gameobject
GameObject::GameObject(int x, int y, char repr, int width, int height,
                       bool gravity, int weight)
    : m_x(x), m_y(y), repr(repr), access(), m_width(width), m_height(height),
      gravityAffects(gravity), weight(weight), accelerationX(0.0),
      accelerationY(0.0), velocityX(0.0), velocityY(0.0) {}

GameObject::GameObject(const GameObject &other)
    : m_x(other.m_x), m_y(other.m_y), repr(other.repr), access(),
      m_width(other.m_width), m_height(other.m_height),
      gravityAffects(other.gravityAffects), weight(other.weight),
      accelerationX(other.accelerationX), accelerationY(other.accelerationY),
      velocityX(other.velocityX), velocityY(other.velocityY) {}

GameObject::GameObject()
    : m_x(0.0), m_y(0.0), repr(' '), access(), m_width(0), m_height(0),
      gravityAffects(true), weight(1), accelerationX(0.0), accelerationY(0.0),
      velocityX(0.0), velocityY(0.0) {}

float GameObject::getX() {
  std::lock_guard<std::mutex> lock(access);
  return m_x;
}

float GameObject::getY() {
  std::lock_guard<std::mutex> lock(access);
  return m_y;
}

void GameObject::setX(float x) {
  std::lock_guard<std::mutex> lock(access);
  m_x = x;
}

void GameObject::setY(float y) {
  std::lock_guard<std::mutex> lock(access);
  m_y = y;
}

int GameObject::getHeight() { return m_height; }
int GameObject::getWidth() { return m_width; }
