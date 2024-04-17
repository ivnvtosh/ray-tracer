#include "PPMExporter.hpp"

int GetColorPixel(int x, int y);

auto height = 512;
auto width = 512;

int main(void) {
  int data[height * width];

  for (auto y = 0; y < height; y += 1) {
    for (auto x = 0; x < width; x += 1) {
      data[y * height + x] = GetColorPixel(x, y);
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

struct Vector3 {
  float x;
  float y;
  float z;
};

struct Ray {
  Vector3 origin;
  Vector3 direction;
};

Ray GetRay(int x, int y);
float SphereIntersection(Ray ray);
Vector3 SphereNormal(Ray ray, float time);

int GetColorPixel(int x, int y) {
  auto ray = GetRay(x, y);

  auto time = SphereIntersection(ray);

  if (time < 0.0f) {
    return 0;
  }

  auto normal = SphereNormal(ray, time);

  normal.x = normal.x < 0.0f ? -normal.x : normal.x;
  normal.y = normal.y < 0.0f ? -normal.y : normal.y;
  normal.z = normal.z < 0.0f ? -normal.z : normal.z;

  return ((int)(normal.x * 255) << 16 | (int)(normal.y * 255) << 8 |
          (int)(normal.z * 255));
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

Vector3 SphereNormal(Ray ray, float time);

float SphereIntersection(Ray ray) {
  auto b = ray.origin.x * ray.direction.x + ray.origin.y * ray.direction.y +
           ray.origin.z * ray.direction.z;

  auto c = ray.origin.x * ray.origin.x + ray.origin.y * ray.origin.y +
           ray.origin.z * ray.origin.z - 1.0f;

  auto h = b * b - c;

  return h < 0.0f ? -1.0f : -b - h;
}

Vector3 SphereNormal(Ray ray, float time) {
  Vector3 result;

  result.x = ray.origin.x + ray.direction.x * time;
  result.y = ray.origin.y + ray.direction.y * time;
  result.z = ray.origin.z + ray.direction.z * time;

  auto length =
      sqrtf(result.x * result.x + result.y * result.y + result.z * result.z);

  result.x /= length;
  result.y /= length;
  result.z /= length;

  return result;
}
