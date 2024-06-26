#ifndef TRIANGLEMESH_HPP
#define TRIANGLEMESH_HPP

#include <string>
#include <vector>

#include "Vector3.hpp"

namespace Engine {

struct Material {
 public:
  Vector3 color;
  std::string name;
  bool isLight;
};

struct Triangle {
 public:
  Vector3 vertices[3];
  Material material;
};

using TriangleMesh = std::vector<Triangle>;

}  // namespace Engine

#endif  // TRIANGLEMESH_HPP
