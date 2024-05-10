#ifndef VECTOR3_HPP
#define VECTOR3_HPP

namespace Engine {

struct Vector3 {
 public:
  float x;
  float y;
  float z;

  static Vector3 Zero() noexcept;

  static float Dot(Vector3, Vector3) noexcept;
  static Vector3 Cross(Vector3, Vector3) noexcept;

  Vector3 Normalized() const noexcept;
  float Length() const noexcept;

  Vector3() noexcept = default;
  Vector3(float x, float y, float z) noexcept;

  bool operator==(Vector3) const noexcept;

  Vector3 operator+(Vector3) const noexcept;
  Vector3 operator-(Vector3) const noexcept;
  Vector3 operator*(Vector3) const noexcept;
  Vector3 operator/(Vector3) const noexcept;

  Vector3& operator+=(Vector3) noexcept;
  Vector3& operator-=(Vector3) noexcept;
  Vector3& operator*=(Vector3) noexcept;
  Vector3& operator/=(Vector3) noexcept;

 private:
  static float eps;
};

Vector3 operator+(float, Vector3) noexcept;
Vector3 operator-(float, Vector3) noexcept;
Vector3 operator*(float, Vector3) noexcept;
Vector3 operator/(Vector3, float) noexcept;

}  // namespace Engine

#endif  // VECTOR3_HPP
