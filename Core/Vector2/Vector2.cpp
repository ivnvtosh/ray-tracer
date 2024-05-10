#include "Vector2.hpp"

#include <cmath>

using namespace Engine;

float Vector2::eps = 0.0001f;

Vector2 Vector2::Zero() noexcept { return {0, 0}; }

Vector2::Vector2(float x, float y) noexcept : x(x), y(y) {}

bool Vector2::operator==(Vector2 other) const noexcept {
  return std::fabs(x - other.x) < eps && std::fabs(y - other.y) < eps;
}

Vector2 Vector2::operator+(Vector2 other) const noexcept {
  return {x + other.x, y + other.y};
}

Vector2 Vector2::operator-(Vector2 other) const noexcept {
  return {x - other.x, y - other.y};
}

Vector2 Vector2::operator*(Vector2 other) const noexcept {
  return {x * other.x, y * other.y};
}

Vector2 Vector2::operator/(Vector2 other) const noexcept {
  return {x / other.x, y / other.y};
}

Vector2& Vector2::operator+=(Vector2 other) noexcept {
  x += other.x;
  y += other.y;
  return *this;
}

Vector2& Vector2::operator-=(Vector2 other) noexcept {
  x -= other.x;
  y -= other.y;
  return *this;
}

Vector2& Vector2::operator*=(Vector2 other) noexcept {
  x *= other.x;
  y *= other.y;
  return *this;
}

Vector2& Vector2::operator/=(Vector2 other) noexcept {
  x /= other.x;
  y /= other.y;
  return *this;
}
