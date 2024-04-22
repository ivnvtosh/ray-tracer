#include "render.h"

int render_pixcel(t_scene *, int x, int y);
t_vector_3 render_reflections(t_scene *, int x, int y);
t_material render_one_times(t_scene *, t_ray *);

t_ray get_ray(t_camera, int x, int y);

float triangle_intersect(t_triangle, t_ray, t_vector_3 *p_point,
                         t_vector_3 *p_normal);
int post_processing(t_scene *, t_vector_3 color);
t_vector_3 get_random_unit_vector_on_hemisphere(unsigned int *seed,
                                                t_vector_3 normal);
t_vector_3 get_reflected_vector(t_vector_3 direction, t_vector_3 normal);

kernel void render(global t_triangle *input, t_camera camera,
                   global int *output) {
  int gid;
  int y;
  int x;
  t_scene scene;

  gid = get_global_id(0);
  y = gid / camera.height;
  x = gid % camera.width;

  scene.camera = camera;

  scene.settings.seed = x * y;
  scene.settings.number_of_rays = 32;
  scene.settings.number_of_reflections = 8;
  scene.settings.gamma = 2.2f;

  scene.number_of_triangles = camera.number;
  scene.triangles = input;

  output[y * camera.height + x] = render_pixcel(&scene, x, y);
}

int render_pixcel(t_scene *scene, int x, int y) {
  t_vector_3 color;
  t_vector_3 result;

  result.x = 0.0f;
  result.y = 0.0f;
  result.z = 0.0f;

  for (int i = 0; i < scene->settings.number_of_rays; i += 1) {
    color = render_reflections(scene, x, y);

    result.x += color.x;
    result.y += color.y;
    result.z += color.z;
  }

  result.x /= 512;
  result.y /= 512;
  result.z /= 512;

  return post_processing(scene, result);
}

static t_vector_3 apply_gamma_correction(t_vector_3 color, float gamma) {
  float inverseGamma = 1.0f / gamma;
  color.x = pow(color.x, inverseGamma);
  color.y = pow(color.y, inverseGamma);
  color.z = pow(color.z, inverseGamma);
  return color;
}

int post_processing(t_scene *scene, t_vector_3 color) {
  color = apply_gamma_correction(color, scene->settings.gamma);

  color.x = min(1.0f, max(0.0f, color.x));
  color.y = min(1.0f, max(0.0f, color.y));
  color.z = min(1.0f, max(0.0f, color.z));

  color.x *= 255;
  color.y *= 255;
  color.z *= 255;

  return (int)(color.x) << 16 | (int)(color.y) << 8 | (int)(color.z);
}

t_vector_3 render_reflections(t_scene *scene, int x, int y) {
  t_vector_3 color;
  t_material material;
  t_ray ray;

  color.x = 1.0f;
  color.y = 1.0f;
  color.z = 1.0f;

  ray = get_ray(scene->camera, x, y);

  for (int i = 0; i < scene->settings.number_of_reflections; i += 1) {
    material = render_one_times(scene, &ray);

    if (material.color.x < 0.0f) {
      break;
    }

    color.x *= material.color.x;
    color.y *= material.color.y;
    color.z *= material.color.z;

    if (material.is_light) {
      color.x *= 100.0f;
      color.y *= 100.0f;
      color.z *= 100.0f;
      return color;
    }
  }

  color.x = 0.0f;
  color.y = 0.0f;
  color.z = 0.0f;

  return color;
}

t_ray get_ray(t_camera camera, int x, int y) {
  t_ray result;
  float length;

  result.origin = camera.position;

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

t_material render_one_times(t_scene *scene, t_ray *ray) {
  t_material material;
  t_triangle hitTriangle;
  float minTime;
  float time;
  int i;
  t_vector_3 point;
  t_vector_3 normal;
  t_vector_3 hitPoint;
  t_vector_3 hitNormal;

  minTime = -1.0f;
  for (int i = 0; i < scene->number_of_triangles; i += 1) {
    time = triangle_intersect(scene->triangles[i], *ray, &point, &normal);

    if (time >= 0.0f && (minTime == -1.0f || time < minTime)) {
      minTime = time;
      hitTriangle = scene->triangles[i];
      hitPoint = point;
      hitNormal = normal;
    }
  }

  if (minTime < 0.0f) {
    material.color.x = -1.0f;
    return material;
  }

  ray->origin.x = hitPoint.x;
  ray->origin.y = hitPoint.y;
  ray->origin.z = hitPoint.z;

  ray->direction =
      get_random_unit_vector_on_hemisphere(&scene->settings.seed, hitNormal);

  return hitTriangle.material;
}

float triangle_intersect(t_triangle triangle, t_ray ray, t_vector_3 *p_point,
                         t_vector_3 *p_normal) {
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

  length = sqrt(length_squared);

  normal.x = cross.x / length;
  normal.y = cross.y / length;
  normal.z = cross.z / length;

  *p_normal = normal;

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

  point.x = ray.origin.x + (time - 0.00001f) * ray.direction.x;
  point.y = ray.origin.y + (time - 0.00001f) * ray.direction.y;
  point.z = ray.origin.z + (time - 0.00001f) * ray.direction.z;

  *p_point = point;

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

static float random(unsigned int *seed) {
  float number;
  *seed = 1664525u * *seed + (unsigned int)seed;
  number = convert_float((*seed & 0x7FFFFFFF) / (float)0x7FFFFFFF);
  return 2.0f * number - 1.0f;
}

t_vector_3 get_random_unit_vector_on_hemisphere(unsigned int *seed,
                                                t_vector_3 normal) {
  t_vector_3 result;
  float length;
  float dot;

  result.x = random(seed);
  result.y = random(seed);
  result.z = random(seed);

  length =
      sqrt(result.x * result.x + result.y * result.y + result.z * result.z);

  result.x /= length;
  result.y /= length;
  result.z /= length;

  dot = result.x * normal.x + result.y * normal.y + result.z * normal.z;

  if (dot < 0) {
    result.x = -result.x;
    result.y = -result.y;
    result.z = -result.z;
  }

  return result;
}

t_vector_3 get_reflected_vector(t_vector_3 direction, t_vector_3 normal) {
  t_vector_3 result;
  float dot2x;

  dot2x = 2.0f * (direction.x * normal.x + direction.y * normal.y +
                  direction.z * normal.z);

  result.x = direction.x - dot2x * normal.x;
  result.y = direction.y - dot2x * normal.y;
  result.z = direction.z - dot2x * normal.z;

  return result;
}
