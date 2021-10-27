#pragma once
#include <mutex>
#include <string>
class GameObject {

public:
  GameObject(float x, float y, char repr, int width = 1, int height = 1,
             bool gravity = true, int weight = 1, std::string tag = "none");
  GameObject(const GameObject &);
  GameObject();
  float getX();
  float getY();
  void setX(float x);
  void setY(float y);
  int getHeight();
  int getWidth();
  char repr;
  bool gravityAffects;
  float velocityX;
  float velocityY;
  float accelerationX;
  float accelerationY;
  int weight;
  std::string tag;

private:
  float m_x;
  float m_y;
  int m_width;
  int m_height;

  std::mutex access;
};
