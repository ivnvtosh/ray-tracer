#ifndef VECTOR2_HPP
#define VECTOR2_HPP

namespace Engine {

struct Vector2 {
 public:
  float x;
  float y;

  static Vector2 Zero() noexcept;

  Vector2() noexcept = default;
  Vector2(float x, float y) noexcept;

  bool operator==(Vector2) const noexcept;

  Vector2 operator+(Vector2) const noexcept;
  Vector2 operator-(Vector2) const noexcept;
  Vector2 operator*(Vector2) const noexcept;
  Vector2 operator/(Vector2) const noexcept;

  Vector2& operator+=(Vector2) noexcept;
  Vector2& operator-=(Vector2) noexcept;
  Vector2& operator*=(Vector2) noexcept;
  Vector2& operator/=(Vector2) noexcept;

 private:
  static float eps;
};

}  // namespace Engine

#endif  // VECTOR2_HPP
