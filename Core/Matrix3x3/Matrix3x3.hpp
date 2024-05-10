#ifndef MATRIX3X3_HPP
#define MATRIX3X3_HPP

#include "Vector3.hpp"

namespace Engine {

struct Matrix3x3 {
 public:
  static Matrix3x3 Unit() noexcept;
  static Vector3 MultiplyByColumnVector(Matrix3x3, Vector3) noexcept;

  Matrix3x3() noexcept = default;

  Matrix3x3 &operator=(const Matrix3x3 &) noexcept;

  float *operator[](int) noexcept;
  const float *operator[](int) const noexcept;

  Matrix3x3 operator*(const Matrix3x3 &) noexcept;

  void Rotate(Vector3 angle);

 private:
  float _data[3][3];

  void RotateX(float angle);
  void RotateY(float angle);
  void RotateZ(float angle);
};

}  // namespace Engine

#endif  // MATRIX3X3_HPP
