#include "Matrix3x3.hpp"

#include <cmath>

using namespace Engine;

Matrix3x3 Matrix3x3::Unit() noexcept {
  Matrix3x3 result;

  result._data[0][0] = 1.0f;
  result._data[0][1] = 0.0f;
  result._data[0][2] = 0.0f;

  result._data[1][0] = 0.0f;
  result._data[1][1] = 1.0f;
  result._data[1][2] = 0.0f;

  result._data[2][0] = 0.0f;
  result._data[2][1] = 0.0f;
  result._data[2][2] = 1.0f;

  return result;
}

Vector3 Matrix3x3::MultiplyByColumnVector(Matrix3x3 a, Vector3 b) noexcept {
  return {a._data[0][0] * b.x + a._data[0][1] * b.y + a._data[0][2] * b.z,
          a._data[1][0] * b.x + a._data[1][1] * b.y + a._data[1][2] * b.z,
          a._data[2][0] * b.x + a._data[2][1] * b.y + a._data[2][2] * b.z};
}

Matrix3x3 &Matrix3x3::operator=(const Matrix3x3 &other) noexcept {
  _data[0][0] = other._data[0][0];
  _data[0][1] = other._data[0][1];
  _data[0][2] = other._data[0][2];

  _data[1][0] = other._data[1][0];
  _data[1][1] = other._data[1][1];
  _data[1][2] = other._data[1][2];

  _data[2][0] = other._data[2][0];
  _data[2][1] = other._data[2][1];
  _data[2][2] = other._data[2][2];

  return *this;
}

float *Matrix3x3::operator[](int index) noexcept { return _data[index]; }

const float *Matrix3x3::operator[](int index) const noexcept {
  return &_data[index][0];
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3 &other) noexcept {
  Matrix3x3 result;

  result._data[0][0] = _data[0][0] * other._data[0][0] +
                       _data[0][1] * other._data[1][0] +
                       _data[0][2] * other._data[2][0];

  result._data[0][1] = _data[0][0] * other._data[0][1] +
                       _data[0][1] * other._data[1][1] +
                       _data[0][2] * other._data[2][1];

  result._data[0][2] = _data[0][0] * other._data[0][2] +
                       _data[0][1] * other._data[1][2] +
                       _data[0][2] * other._data[2][2];

  result._data[1][0] = _data[1][0] * other._data[0][0] +
                       _data[1][1] * other._data[1][0] +
                       _data[1][2] * other._data[2][0];

  result._data[1][1] = _data[1][0] * other._data[0][1] +
                       _data[1][1] * other._data[1][1] +
                       _data[1][2] * other._data[2][1];

  result._data[1][2] = _data[1][0] * other._data[0][2] +
                       _data[1][1] * other._data[1][2] +
                       _data[1][2] * other._data[2][2];

  result._data[2][0] = _data[2][0] * other._data[0][0] +
                       _data[2][1] * other._data[1][0] +
                       _data[2][2] * other._data[2][0];

  result._data[2][1] = _data[2][0] * other._data[0][1] +
                       _data[2][1] * other._data[1][1] +
                       _data[2][2] * other._data[2][1];

  result._data[2][2] = _data[2][0] * other._data[0][2] +
                       _data[2][1] * other._data[1][2] +
                       _data[2][2] * other._data[2][2];

  return result;
}

void Matrix3x3::Rotate(Vector3 angle) {
  *this = Matrix3x3::Unit();
  RotateZ(angle.z);
  RotateX(angle.x);
  RotateY(angle.y);
}

void Matrix3x3::RotateX(float angle) {
  Matrix3x3 temporary;

  auto radian = M_PI / 180.0f * angle;
  auto cos = std::cosf(radian);
  auto sin = std::sinf(radian);

  temporary._data[0][0] = 1.0f;
  temporary._data[0][1] = 0.0f;
  temporary._data[0][2] = 0.0f;

  temporary._data[1][0] = 0.0f;
  temporary._data[1][1] = cos;
  temporary._data[1][2] = sin;

  temporary._data[2][0] = 0.0f;
  temporary._data[2][1] = -sin;
  temporary._data[2][2] = cos;

  *this = *this * temporary;
}

void Matrix3x3::RotateY(float angle) {
  Matrix3x3 temporary;

  auto radian = M_PI / 180.0f * angle;
  auto cos = std::cosf(radian);
  auto sin = std::sinf(radian);

  temporary._data[0][0] = cos;
  temporary._data[0][1] = 0.0f;
  temporary._data[0][2] = -sin;

  temporary._data[1][0] = 0.0f;
  temporary._data[1][1] = 1.0f;
  temporary._data[1][2] = 0.0f;

  temporary._data[2][0] = sin;
  temporary._data[2][1] = 0.0f;
  temporary._data[2][2] = cos;

  *this = *this * temporary;
}

void Matrix3x3::RotateZ(float angle) {
  Matrix3x3 temporary;

  auto radian = M_PI / 180.0f * angle;
  auto cos = std::cosf(radian);
  auto sin = std::sinf(radian);

  temporary._data[0][0] = cos;
  temporary._data[0][1] = sin;
  temporary._data[0][2] = 0.0f;

  temporary._data[1][0] = -sin;
  temporary._data[1][1] = cos;
  temporary._data[1][2] = 0.0f;

  temporary._data[2][0] = 0.0f;
  temporary._data[2][1] = 0.0f;
  temporary._data[2][2] = 1.0f;

  *this = *this * temporary;
}
