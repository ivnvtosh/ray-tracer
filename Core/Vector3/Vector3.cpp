#include "Vector3.hpp"

#include <cmath>

using namespace Engine;

float Vector3::eps = 0.0001f;

Vector3 Vector3::Zero() noexcept { return {0, 0, 0}; }

Vector3::Vector3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

float Vector3::Dot(Vector3 a, Vector3 b) noexcept {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::Cross(Vector3 a, Vector3 b) noexcept {
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

Vector3 Vector3::Normalized() const noexcept {
  return x == 0.0f && y == 0.0f && z == 0.0f ? *this : *this / Length();
}

float Vector3::Length() const noexcept {
  return std::sqrtf(x * x + y * y + z * z);
}

bool Vector3::operator==(Vector3 other) const noexcept {
  return std::fabs(x - other.x) < eps && std::fabs(y - other.y) < eps &&
         std::fabs(z - other.z) < eps;
}

Vector3 Vector3::operator+(Vector3 other) const noexcept {
  return {x + other.x, y + other.y, z + other.z};
}

Vector3 Vector3::operator-(Vector3 other) const noexcept {
  return {x - other.x, y - other.y, z - other.z};
}

Vector3 Vector3::operator*(Vector3 other) const noexcept {
  return {x * other.x, y * other.y, z * other.z};
}

Vector3 Vector3::operator/(Vector3 other) const noexcept {
  return {x / other.x, y / other.y, z / other.z};
}

Vector3& Vector3::operator+=(Vector3 other) noexcept {
  x += other.x;
  y += other.y;
  z += other.z;
  return *this;
}

Vector3& Vector3::operator-=(Vector3 other) noexcept {
  x -= other.x;
  y -= other.y;
  z -= other.z;
  return *this;
}

Vector3& Vector3::operator*=(Vector3 other) noexcept {
  x *= other.x;
  y *= other.y;
  z *= other.z;
  return *this;
}

Vector3& Vector3::operator/=(Vector3 other) noexcept {
  x /= other.x;
  y /= other.y;
  z /= other.z;
  return *this;
}

Vector3 Engine::operator+(float a, Vector3 b) noexcept {
  return {a + b.x, a + b.y, a + b.z};
}

Vector3 Engine::operator-(float a, Vector3 b) noexcept {
  return {a - b.x, a - b.y, a - b.z};
}

Vector3 Engine::operator*(float a, Vector3 b) noexcept {
  return {a * b.x, a * b.y, a * b.z};
}

Vector3 Engine::operator/(Vector3 a, float b) noexcept {
  return {a.x / b, a.y / b, a.z / b};
}
