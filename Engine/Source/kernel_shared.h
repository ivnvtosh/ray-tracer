#ifndef KERNEL_SHARED_H
#define KERNEL_SHARED_H

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
  unsigned int seed;
};

typedef struct s_vector_3 t_vector_3;
typedef struct s_material t_material;
typedef struct s_triangle t_triangle;
typedef struct s_camera t_camera;

#endif  // KERNEL_SHARED_H
