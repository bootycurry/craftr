#include "craftr_engine/vector2.hpp"

Vector2::Vector2(int x, int y) {
  this->x = x;
  this->y = y;
}

Vector2::Vector2() {
  this->x = 0;
  this->y = 0;
}

Vector2 Vector2::operator*(const int &other) const {
  return Vector2(this->x * other, this->y * other);
}

Vector2 operator*(const int &scalar, const Vector2 &vec) {
  return vec * scalar;
}

Vector2 Vector2::operator+(const Vector2 &other) const {
  return Vector2(this->x + other.x, this->y + other.y);
}

Vector2 Vector2::operator-(const Vector2 &other) const {
  return Vector2(this->x - other.x, this->y - other.y);
}
