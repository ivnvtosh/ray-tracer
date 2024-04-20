#include "render.h"

int render_pixcel(global t_triangle *, t_camera, int x, int y);
t_ray get_ray(t_camera camera, int x, int y);
t_vector_3 render_one_times(global t_triangle *input, int number, t_ray ray);
float triangle_intersect(t_triangle, t_ray);
int post_processing(t_vector_3 color);

kernel void render(global t_triangle *input, t_camera camera,
                   global int *output) {
  int gid;
  int y;
  int x;

  gid = get_global_id(0);
  y = gid / camera.height;
  x = gid % camera.width;

  output[y * camera.height + x] = render_pixcel(input, camera, x, y);
}

int render_pixcel(global t_triangle *input, t_camera camera, int x, int y) {
  t_ray ray;
  t_vector_3 color;

  ray = get_ray(camera, x, y);
  color = render_one_times(input, camera.number, ray);

  return post_processing(color);
}

t_vector_3 render_one_times(global t_triangle *input, int number, t_ray ray) {
  t_vector_3 color;
  t_triangle hitTriangle;
  float minTime;
  float time;
  int i;

  minTime = -1.0f;
  i = 0;
  while (i < number) {
    time = triangle_intersect(input[i], ray);

    if (time >= 0.0f && (minTime == -1.0f || time < minTime)) {
      minTime = time;
      hitTriangle = input[i];
    }

    i += 1;
  }

  if (minTime < 0.0f) {
    color.x = 0.0f;
    color.y = 0.0f;
    color.z = 0.0f;
  } else {
    color = hitTriangle.material.color;
  }

  return color;
}

t_ray get_ray(t_camera camera, int x, int y) {
  t_ray result;
  float length;

  result.origin.x = camera.position.x;
  result.origin.y = camera.position.y;
  result.origin.z = camera.position.z;

  result.direction.z = -(camera.focus);
  result.direction.y = -(y - camera.height / 2.0f);
  result.direction.x = +(x - camera.width / 2.0f);

  length = sqrt(result.direction.x * result.direction.x +
                result.direction.y * result.direction.y +
                result.direction.z * result.direction.z);

  result.direction.x /= length;
  result.direction.y /= length;
  result.direction.z /= length;

  return result;
}

int post_processing(t_vector_3 color) {
  color.x = min(1.0f, max(0.0f, color.x)) * 255;
  color.y = min(1.0f, max(0.0f, color.y)) * 255;
  color.z = min(1.0f, max(0.0f, color.z)) * 255;

  return (int)(color.x) << 16 | (int)(color.y) << 8 | (int)(color.z);
}

float triangle_intersect(t_triangle triangle, t_ray ray) {
  t_vector_3 sub_1;
  t_vector_3 sub_2;
  t_vector_3 cross;
  float length_squared;
  t_vector_3 normal;
  float length;
  float dot;
  float denominator;
  float abs_denominator;
  float numerator;
  float time;
  t_vector_3 point;
  t_vector_3 sub_3;
  float dot00;
  float dot01;
  float dot02;
  float dot11;
  float dot12;
  float invDenom;
  float u;
  float v;

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
    return -1.0f;
  }

  length = sqrt(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z);

  normal.x = cross.x / length;
  normal.y = cross.y / length;
  normal.z = cross.z / length;

  denominator = ray.direction.x * normal.x + ray.direction.y * normal.y +
                ray.direction.z * normal.z;

  abs_denominator = denominator < 0.0f ? -denominator : denominator;

  if (abs_denominator < 0.00001f) {
    return -1.0f;
  }

  numerator = (triangle.vertices[0].x - ray.origin.x) * normal.x +
              (triangle.vertices[0].y - ray.origin.y) * normal.y +
              (triangle.vertices[0].z - ray.origin.z) * normal.z;

  time = numerator / denominator;

  if (time < 0.0f) {
    return -1.0f;
  }

  point.x = ray.origin.x + time * ray.direction.x;
  point.y = ray.origin.y + time * ray.direction.y;
  point.z = ray.origin.z + time * ray.direction.z;

  sub_3.x = point.x - triangle.vertices[0].x;
  sub_3.y = point.y - triangle.vertices[0].y;
  sub_3.z = point.z - triangle.vertices[0].z;

  dot00 = sub_1.x * sub_1.x + sub_1.y * sub_1.y + sub_1.z * sub_1.z;
  dot01 = sub_1.x * sub_2.x + sub_1.y * sub_2.y + sub_1.z * sub_2.z;
  dot02 = sub_1.x * sub_3.x + sub_1.y * sub_3.y + sub_1.z * sub_3.z;
  dot11 = sub_2.x * sub_2.x + sub_2.y * sub_2.y + sub_2.z * sub_2.z;
  dot12 = sub_2.x * sub_3.x + sub_2.y * sub_3.y + sub_2.z * sub_3.z;

  invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
  u = (dot11 * dot02 - dot01 * dot12) * invDenom;
  v = (dot00 * dot12 - dot01 * dot02) * invDenom;

  if ((u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f)) {
    return time;
  } else {
    return -1.0f;
  }
}
