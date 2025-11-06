#ifndef CRAFTR_VECTOR2_HPP
#define CRAFTR_VECTOR2_HPP

class Vector2 {
public:
  float x;
  float y;

  Vector2 operator*(const int &other) const;
  Vector2 operator+(const Vector2 &other) const;
  Vector2 operator-(const Vector2 &other) const;

  Vector2(float x, float y);
  Vector2();
};

Vector2 operator*(const int &scalar, const Vector2 &vec);

#endif
