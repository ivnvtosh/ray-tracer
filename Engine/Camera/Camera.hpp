#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Matrix3x3.hpp"
#include "Size.hpp"
#include "Vector3.hpp"

namespace Engine {

struct Camera {
 public:
  Size size;
  Vector3 position;
  Vector3 rotation;
  Matrix3x3 matrix;

  float GetFov() const noexcept;
  void SetFov(float) noexcept;
  float GetFocus() const noexcept;

  void Rotate(Vector3 angle) noexcept;
  void AddPositionByDirection(Vector3) noexcept;

 private:
  float _fov;
  float _focus;
};

}  // namespace Engine

#endif  // CAMERA_HPP
