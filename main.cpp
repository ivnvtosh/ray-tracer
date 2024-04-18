#include <iostream>
#include <vector>

#include "PPMExporter.hpp"

struct Vector3 {
 public:
  float x;
  float y;
  float z;
};

struct Triangle {
 public:
  Vector3 v0;
  Vector3 v1;
  Vector3 v2;
};

int GetColorPixel(const std::vector<Triangle>& triangles, int x, int y);

auto height = 512;
auto width = 512;

int main(void) {
  int data[height * width];

  std::vector<Triangle> triangles;

  Triangle triangle;

  triangle.v0.x = -1;
  triangle.v0.y = -1;
  triangle.v0.z = -1;

  triangle.v1.x = 0;
  triangle.v1.y = 1;
  triangle.v1.z = 0;

  triangle.v2.x = 0;
  triangle.v2.y = 0;
  triangle.v2.z = 1;

  triangles.push_back(triangle);

  for (auto y = 0; y < height; y += 1) {
    for (auto x = 0; x < width; x += 1) {
      data[y * height + x] = GetColorPixel(triangles, x, y);
    }
  }

  auto request = Engine::PPMRequest()
                     .Filepath("../../")
                     .Filename("image")
                     .Height(height)
                     .Width(width)
                     .Data(data);

  Engine::PPMExporter().Export(request);

  return 0;
}

struct Ray {
 public:
  Vector3 origin;
  Vector3 direction;
};

Vector3 operator+(Vector3 left, Vector3 right) {
  left.x += right.x;
  left.y += right.y;
  left.z += right.z;
  return left;
}

Vector3 operator-(Vector3 left, Vector3 right) {
  left.x -= right.x;
  left.y -= right.y;
  left.z -= right.z;
  return left;
}

Vector3 operator*(Vector3 left, Vector3 right) {
  left.x *= right.x;
  left.y *= right.y;
  left.z *= right.z;
  return left;
}

Vector3 operator*(Vector3 left, float right) {
  left.x *= right;
  left.y *= right;
  left.z *= right;
  return left;
}

Vector3 cross(Vector3 left, Vector3 right) {
  Vector3 result;

  result.x = left.y * right.z - left.z * right.y;
  result.y = left.z * right.x - left.x * right.z;
  result.z = left.x * right.y - left.y * right.x;

  return result;
}

float dot(Vector3 left, Vector3 right) {
  return left.x * right.x + left.y * right.y + left.z * right.z;
}

Vector3 normalize(Vector3 vector) {
  auto length =
      sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

  vector.x /= length;
  vector.y /= length;
  vector.z /= length;

  return vector;
}

Ray GetRay(int x, int y);
int IntersectTriangleMesh(Ray, const std::vector<Triangle>&);

int GetColorPixel(const std::vector<Triangle>& triangles, int x, int y) {
  auto ray = GetRay(x, y);

  return IntersectTriangleMesh(ray, triangles);
}

Ray GetRay(int x, int y) {
  Ray result;

  result.origin.x = -4.0f;
  result.origin.y = 0.0f;
  result.origin.z = 0.0f;

  auto fov = 45.0f;
  auto focus = height / 2 / tanf(fov / 2.0f * M_PI / 180.0f);

  result.direction.x = focus;
  result.direction.y = x - width / 2.0f;
  result.direction.z = y - height / 2.0f;

  auto length = sqrtf(result.direction.x * result.direction.x +
                      result.direction.y * result.direction.y +
                      result.direction.z * result.direction.z);

  result.direction.x /= length;
  result.direction.y /= length;
  result.direction.z /= length;

  return result;
}

bool TriangleIntersect(Ray ray, Triangle triangle, float& t) {
  const float EPSILON = 0.000001;
  Vector3 edge1, edge2, h, s, q;
  float a, f, u, v;

  edge1 = triangle.v1 - triangle.v0;
  edge2 = triangle.v2 - triangle.v0;

  h = cross(ray.direction, edge2);
  a = dot(edge1, h);

  if (a > -EPSILON && a < EPSILON) return false;

  f = 1.0f / a;
  s = ray.origin - triangle.v0;
  u = f * dot(s, h);

  if (u < 0.0 || u > 1.0) return false;

  q = cross(s, edge1);
  v = f * dot(ray.direction, q);

  if (v < 0.0 || u + v > 1.0) return false;

  t = f * dot(edge2, q);

  if (t > EPSILON) return true;

  return false;
}

int IntersectTriangleMesh(Ray ray, const std::vector<Triangle>& triangles) {
  bool intersection = false;
  float time = std::numeric_limits<float>::max();
  Triangle triangle;

  for (const Triangle& currentTriangle : triangles) {
    float tmp;
    if (TriangleIntersect(ray, currentTriangle, tmp) && tmp < time) {
      time = tmp;
      intersection = true;
      triangle = currentTriangle;
    }
  }

  if (!intersection) {
    return 0;
  }

  Vector3 edge1 = triangle.v1 - triangle.v0;
  Vector3 edge2 = triangle.v2 - triangle.v0;
  Vector3 normal = normalize(cross(edge1, edge2));

  normal.x = normal.x < 0.0f ? -normal.x : normal.x;
  normal.y = normal.y < 0.0f ? -normal.y : normal.y;
  normal.z = normal.z < 0.0f ? -normal.z : normal.z;

  return ((int)(normal.x * 255) << 16 | (int)(normal.y * 255) << 8 |
          (int)(normal.z * 255));
}
