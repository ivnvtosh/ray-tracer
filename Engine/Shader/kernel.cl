#include "kernel.h"

kernel void render(global t_triangle *input, t_camera camera,
                   global int *output) {
  int gid;
  int y;
  int x;
  t_scene scene;

  gid = get_global_id(0);
  y = gid / camera.oy + camera.y;
  x = gid % camera.ox + camera.x;

  scene.camera = camera;
  scene.number_of_triangles = camera.number;
  scene.triangles = input;

  output[y * camera.height + x] = render_pixcel(&scene, x, y);
}

int render_pixcel(t_scene *scene, int x, int y) {
  t_vector_3 color;
  t_ray ray;
  t_hit_info info;

  ray = get_ray(scene->camera, x, y);
  info = render_one_times(scene, ray);

  if (!info.is_hit) {
    color.x = 0.0f;
    color.y = 0.0f;
    color.z = 0.0f;
    return post_processing(scene, color);
  }

  // ray.direction.x *= -10.0f;
  // ray.direction.y *= -10.0f;
  // ray.direction.z *= -10.0f;

  ray.origin.x = -ray.direction.x;
  ray.origin.y = -ray.direction.y;
  ray.origin.z = -ray.direction.z;

  // float length;

  // length = sqrt(ray.origin.x * ray.origin.x + ray.origin.y * ray.origin.y +
  //               ray.origin.z * ray.origin.z);

  // ray.origin.x /= length;
  // ray.origin.y /= length;
  // ray.origin.z /= length;

  float dot;

  dot = ray.origin.x * info.normal.x + ray.origin.y * info.normal.y +
        ray.origin.z * info.normal.z;

  dot = dot < 0 ? -dot : dot;

  color.x = info.material.color.x * dot;
  color.y = info.material.color.y * dot;
  color.z = info.material.color.z * dot;

  return post_processing(scene, color);
}

int post_processing(t_scene *scene, t_vector_3 color) {
  color.x = min(1.0f, max(0.0f, color.x));
  color.y = min(1.0f, max(0.0f, color.y));
  color.z = min(1.0f, max(0.0f, color.z));

  color.x *= 255;
  color.y *= 255;
  color.z *= 255;

  return 0 << 24 | (int)(color.x) << 16 | (int)(color.y) << 8 | (int)(color.z);
}

t_ray get_ray(t_camera camera, int x, int y) {
  t_ray result;
  float length;

  result.origin = camera.position;

  t_vector_3 direction;

  direction.x = (camera.focus);
  direction.y = (x - camera.width / 2.0f);
  direction.z = (y - camera.height / 2.0f);

  result.direction.x = camera.a11 * direction.x + camera.a12 * direction.y +
                       camera.a13 * direction.z;

  result.direction.y = camera.a21 * direction.x + camera.a22 * direction.y +
                       camera.a23 * direction.z;

  result.direction.z = camera.a31 * direction.x + camera.a32 * direction.y +
                       camera.a33 * direction.z;

  length = sqrt(result.direction.x * result.direction.x +
                result.direction.y * result.direction.y +
                result.direction.z * result.direction.z);

  result.direction.x /= length;
  result.direction.y /= length;
  result.direction.z /= length;

  return result;
}

t_hit_info render_one_times(t_scene *scene, t_ray ray) {
  t_hit_info info;
  t_hit_info current;

  info.is_hit = false;

  for (int i = 0; i < scene->number_of_triangles; i += 1) {
    current = triangle_intersect(scene->triangles[i], ray);

    if (current.is_hit && (!info.is_hit || current.time < info.time)) {
      info = current;
    }
  }

  return info;
}

t_hit_info triangle_intersect(t_triangle triangle, t_ray ray) {
  t_hit_info info;
  t_vector_3 sub_1;
  t_vector_3 sub_2;
  t_vector_3 cross;
  float length_squared;
  float length;
  float denominator;
  float numerator;
  t_vector_3 sub_3;
  float dot00;
  float dot01;
  float dot02;
  float dot11;
  float dot12;
  float invDenom;
  float u;
  float v;

  info.material = triangle.material;
  info.is_hit = false;

  sub_1.x = triangle.vertices[1].x - triangle.vertices[0].x;
  sub_1.y = triangle.vertices[1].y - triangle.vertices[0].y;
  sub_1.z = triangle.vertices[1].z - triangle.vertices[0].z;

  sub_2.x = triangle.vertices[2].x - triangle.vertices[0].x;
  sub_2.y = triangle.vertices[2].y - triangle.vertices[0].y;
  sub_2.z = triangle.vertices[2].z - triangle.vertices[0].z;

  cross.x = sub_1.y * sub_2.z - sub_1.z * sub_2.y;
  cross.y = sub_1.z * sub_2.x - sub_1.x * sub_2.z;
  cross.z = sub_1.x * sub_2.y - sub_1.y * sub_2.x;

  length_squared = cross.x * cross.x + cross.y * cross.y + cross.z * cross.z;

  if (length_squared < 0.00001f) {
    return info;
  }

  length = sqrt(length_squared);

  info.normal.x = cross.x / length;
  info.normal.y = cross.y / length;
  info.normal.z = cross.z / length;

  denominator = ray.direction.x * info.normal.x +
                ray.direction.y * info.normal.y +
                ray.direction.z * info.normal.z;

  if ((denominator < 0.0f ? -denominator : denominator) < 0.00001f) {
    return info;
  }

  numerator = (triangle.vertices[0].x - ray.origin.x) * info.normal.x +
              (triangle.vertices[0].y - ray.origin.y) * info.normal.y +
              (triangle.vertices[0].z - ray.origin.z) * info.normal.z;

  info.time = numerator / denominator;

  if (info.time < 0.0f) {
    return info;
  }

  info.point.x = ray.origin.x + (info.time - 0.00001f) * ray.direction.x;
  info.point.y = ray.origin.y + (info.time - 0.00001f) * ray.direction.y;
  info.point.z = ray.origin.z + (info.time - 0.00001f) * ray.direction.z;

  sub_3.x = info.point.x - triangle.vertices[0].x;
  sub_3.y = info.point.y - triangle.vertices[0].y;
  sub_3.z = info.point.z - triangle.vertices[0].z;

  dot00 = sub_1.x * sub_1.x + sub_1.y * sub_1.y + sub_1.z * sub_1.z;
  dot01 = sub_1.x * sub_2.x + sub_1.y * sub_2.y + sub_1.z * sub_2.z;
  dot02 = sub_1.x * sub_3.x + sub_1.y * sub_3.y + sub_1.z * sub_3.z;
  dot11 = sub_2.x * sub_2.x + sub_2.y * sub_2.y + sub_2.z * sub_2.z;
  dot12 = sub_2.x * sub_3.x + sub_2.y * sub_3.y + sub_2.z * sub_3.z;

  invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
  u = (dot11 * dot02 - dot01 * dot12) * invDenom;
  v = (dot00 * dot12 - dot01 * dot02) * invDenom;

  info.is_hit = (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);

  return info;
}
