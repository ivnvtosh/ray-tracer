#ifndef KERNEL_H
#define KERNEL_H

#ifndef __OPENCL_C_VERSION__
#include <math.h>
#endif

#include "kernel_shared.h"

struct s_ray {
  struct s_vector_3 origin;
  struct s_vector_3 direction;
};

struct s_hit_info {
  bool is_hit;
  float time;
  struct s_material material;
  struct s_vector_3 point;
  struct s_vector_3 normal;
  struct s_ray reflected_ray;
};

struct s_settings {
  unsigned int seed;
  int number_of_rays;
  int number_of_reflections;
  float gamma;
};

struct s_scene {
  struct s_camera camera;
  struct s_settings settings;
  int number_of_triangles;
  global struct s_triangle *triangles;
};

typedef struct s_ray t_ray;
typedef struct s_hit_info t_hit_info;
typedef struct s_scene t_scene;


int render_pixcel(t_scene *, int x, int y);
t_vector_3 render_reflections(t_scene *, int x, int y);
t_hit_info render_one_times(t_scene *, t_ray);

t_ray get_ray(t_camera, int x, int y);
int post_processing(t_scene *, t_vector_3 color);

t_hit_info triangle_intersect(t_triangle, t_ray);

t_vector_3 get_random_unit_vector_on_hemisphere(unsigned int *seed,
                                                t_vector_3 normal);
t_vector_3 get_reflected_vector(t_vector_3 direction, t_vector_3 normal);


#endif  // KERNEL_H