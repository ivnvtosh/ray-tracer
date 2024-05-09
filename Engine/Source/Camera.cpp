#include "Camera.hpp"

#include <cmath>

using namespace Engine;

float Camera::GetFov() const noexcept { return _fov; }

void Camera::SetFov(float fov) noexcept {
  _fov = fov;
  _focus = width / 2.0f / std::tanf(fov / 2.0f * M_PI / 180.0f);
}

float Camera::GetFocus() const noexcept { return _focus; }

void Camera::Rotate(Vector3 angle) noexcept {
  if (angle == rotation) {
    return;
  }

  rotation = angle;
  matrix.Rotate(angle);
}

void Camera::AddPositionByDirection(Vector3 move) noexcept {
  if (move == Vector3::Zero()) {
    return;
  }

  position += Matrix3x3::MultiplyByColumnVector(matrix, move);
}
