#include "Render.hpp"

using namespace Engine;

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

Vector3 operator*(float left, Vector3 right) {
  right.x *= left;
  right.y *= left;
  right.z *= left;
  return right;
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

float lengthSquared(Vector3 vector) {
  return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
}

void Render::Rendering(int* data) {
  for (auto y = 0; y < camera.height; y += 1) {
    for (auto x = 0; x < camera.width; x += 1) {
      data[y * camera.height + x] = GetColorPixel(x, y);
    }
  }
}

int Render::GetColorPixel(int x, int y) {
  auto ray = GetRay(x, y);

  Engine::Triangle hitTriangle;
  float time = std::numeric_limits<float>::max();
  bool hit = false;

  for (auto triangle : mesh) {
    float t;
    if (RayPolygonIntersect(ray, triangle, t) && t < time) {
      time = t;
      hitTriangle = triangle;
      hit = true;
    }
  }

  if (!hit) {
    return 0;
  }

  Engine::Vector3 color = hitTriangle.material.color;

  color.x = std::min(1.0f, std::max(0.0f, color.x)) * 255;
  color.y = std::min(1.0f, std::max(0.0f, color.y)) * 255;
  color.z = std::min(1.0f, std::max(0.0f, color.z)) * 255;

  return (int)(color.x) << 16 | (int)(color.y) << 8 | (int)(color.z);
}

Render::Ray Render::GetRay(int x, int y) {
  Ray result;

  result.origin.x = -0.01f;
  result.origin.y = 0.99f;
  result.origin.z = 3.39f;

  result.direction.z = -camera.focus;
  result.direction.y = -(y - camera.height / 2.0f);
  result.direction.x = (x - camera.width / 2.0f);

  auto length = sqrtf(result.direction.x * result.direction.x +
                      result.direction.y * result.direction.y +
                      result.direction.z * result.direction.z);

  result.direction.x /= length;
  result.direction.y /= length;
  result.direction.z /= length;

  return result;
}

bool Render::RayPolygonIntersect(const Ray& ray, const Triangle& triangle,
                                 float& t) {
  if (lengthSquared(cross(triangle.vertices[1] - triangle.vertices[0],
                          triangle.vertices[2] - triangle.vertices[0])) <
      0.00001f) {
    return false;
  }

  Vector3 normal =
      normalize(cross(triangle.vertices[1] - triangle.vertices[0],
                      triangle.vertices[2] - triangle.vertices[0]));

  float denominator = dot(ray.direction, normal);
  if (std::abs(denominator) < 0.00001f) {
    return false;
  }

  float numerator = dot(triangle.vertices[0] - ray.origin, normal);
  t = numerator / denominator;

  if (t < 0.0f) {
    return false;
  }

  Vector3 point = ray.origin + t * ray.direction;
  Vector3 v0v1 = triangle.vertices[1] - triangle.vertices[0];
  Vector3 v0v2 = triangle.vertices[2] - triangle.vertices[0];
  Vector3 v0p = point - triangle.vertices[0];

  float dot00 = dot(v0v1, v0v1);
  float dot01 = dot(v0v1, v0v2);
  float dot02 = dot(v0v1, v0p);
  float dot11 = dot(v0v2, v0v2);
  float dot12 = dot(v0v2, v0p);

  float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
  float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
  float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

  return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
}
