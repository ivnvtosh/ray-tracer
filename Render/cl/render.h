#ifndef RENDER_H
#define RENDER_H

#ifndef __OPENCL_C_VERSION__
#include <math.h>
#endif

struct s_vector_3 {
  float x;
  float y;
  float z;
};

struct s_material {
  struct s_vector_3 color;
  int is_light;
};

struct s_triangle {
  struct s_vector_3 vertices[3];
  struct s_material material;
};

struct s_camera {
  int height;
  int width;
  float fov;
  float focus;
  struct s_vector_3 position;
  int number;
};

struct s_ray {
  struct s_vector_3 origin;
  struct s_vector_3 direction;
};

typedef struct s_vector_3 t_vector_3;
typedef struct s_material t_material;
typedef struct s_triangle t_triangle;
typedef struct s_camera t_camera;
typedef struct s_ray t_ray;

#endif  // RENDER_H
