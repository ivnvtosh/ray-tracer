#ifndef RENDER_HPP
#define RENDER_HPP

#include "TriangleMesh.hpp"

namespace Engine {

struct Camera {
 public:
  int height;
  int width;
  float fov;
  float focus;
};

class Render {
 public:
  Camera camera;
  TriangleMesh mesh;

  void Rendering(int* data);

 private:
  struct Ray {
    Vector3 origin;
    Vector3 direction;
  };

  int GetColorPixel(int x, int y);
  Ray GetRay(int x, int y);
  bool RayPolygonIntersect(const Ray&, const Triangle&, float& t);
};

}  // namespace Engine

#endif  // RENDER_HPP
