#include "Render.hpp"

#include <iostream>

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

struct s_ray {
  struct s_vector_3 origin;
  struct s_vector_3 direction;
};

void Engine::Render::Setup() {
  GetDeviceId();
  CreateContext();
  CreateCommandQueue();
  CreateKernel();
  LoadMem();
  SetArgument();
}

void Engine::Render::Rendering(int *data) {
  int err;

  struct s_camera c_camera;

  c_camera.height = camera.height;
  c_camera.width = camera.width;
  c_camera.fov = camera.fov;
  c_camera.focus = camera.focus;
  c_camera.position.x = camera.position.x;
  c_camera.position.y = camera.position.y;
  c_camera.position.z = camera.position.z;
  c_camera.number = mesh.size();
  c_camera.seed = rand();
  
  err = clSetKernelArg(kernel, 1, sizeof(struct s_camera), &c_camera);
  if (err != 0) {
    throw;
  }

  size_t items = camera.height * camera.width;

  err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &items, NULL, 0, NULL,
                               NULL);
  if (err != 0) {
    throw;
  }

  auto bytes = camera.height * camera.width * sizeof(int);
  err = clEnqueueReadBuffer(queue, output, CL_TRUE, 0, bytes, data, 0, NULL,
                            NULL);
  if (err != 0) {
    throw;
  }
}

void Engine::Render::GetDeviceId() {
  int err;

  err = clGetPlatformIDs(1, &platform, NULL);
  if (err != 0) {
    throw;
  }

  err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
  if (err != 0) {
    throw;
  }
}

void Engine::Render::CreateContext() {
  int err;

  context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
  if (err != 0) {
    throw;
  }
}

void Engine::Render::CreateCommandQueue() {
  int err;

  queue = clCreateCommandQueue(context, device, 0, &err);
  if (err != 0) {
    throw;
  }
}

void Engine::Render::CreateKernel() {
  int err;

  auto program = CreateProgram();

  err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
  if (err != 0) {
    char *log;
    size_t size;
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL,
                          &size);
    log = (char *)malloc(size);
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, size, log,
                          NULL);

    std::cout << log << std::endl;

    free(log);
  }

  kernel = clCreateKernel(program, "render", &err);
  if (err != 0) {
    throw;
  }
}

cl_program Engine::Render::CreateProgram() {
  cl_program program;
  char **program_buf;
  size_t *program_size;
  int files_num;
  int err;

  files_num = 1;

  program_buf = (char **)malloc(sizeof(char *) * files_num);
  program_size = (size_t *)malloc(sizeof(size_t) * files_num);
  ReadFilesToBuffer(program_buf, program_size);
  program =
      clCreateProgramWithSource(context, files_num, (const char **)program_buf,
                                (const size_t *)program_size, &err);
  if (err != 0) {
    throw;
  }

  err = -1;

  while (++err < files_num) {
    free(program_buf[err]);
  }

  free(program_buf);
  free(program_size);

  return program;
}

void Engine::Render::ReadFilesToBuffer(char **program_buf,
                                       size_t *program_size) {
  program_buf[0] = ReadFileToBuffer("../Render/kernel.cl", &program_size[3]);
}

char *Engine::Render::ReadFileToBuffer(const char *name, size_t *program_size) {
  size_t file_size;
  FILE *fd;
  char *buf;

  fd = fopen(name, "r");
  if (!fd) throw;
  fseek(fd, 0, SEEK_END);
  file_size = ftell(fd);
  rewind(fd);
  buf = (char *)malloc(file_size + 1);
  fread(buf, sizeof(char), file_size, fd);
  buf[file_size] = '\0';
  fclose(fd);
  *program_size = file_size;

  return buf;
}

void Engine::Render::LoadMem() {
  int err;

  output =
      clCreateBuffer(context, CL_MEM_READ_WRITE,
                     camera.height * camera.width * sizeof(int), NULL, &err);
  if (err != 0) {
    throw;
  }

  auto size = mesh.size() * sizeof(struct s_triangle);
  input = clCreateBuffer(context, CL_MEM_READ_ONLY, size, NULL, &err);
  if (err != 0) {
    throw;
  }

  struct s_triangle *triangles =
      (struct s_triangle *)malloc(mesh.size() * sizeof(struct s_triangle));

  for (auto i = 0u; i < mesh.size(); i += 1) {
    triangles[i].material.color.x = mesh[i].material.color.x;
    triangles[i].material.color.y = mesh[i].material.color.y;
    triangles[i].material.color.z = mesh[i].material.color.z;

    triangles[i].material.is_light = mesh[i].material.isLight;

    triangles[i].vertices[0].x = mesh[i].vertices[0].x;
    triangles[i].vertices[0].y = mesh[i].vertices[0].y;
    triangles[i].vertices[0].z = mesh[i].vertices[0].z;

    triangles[i].vertices[1].x = mesh[i].vertices[1].x;
    triangles[i].vertices[1].y = mesh[i].vertices[1].y;
    triangles[i].vertices[1].z = mesh[i].vertices[1].z;

    triangles[i].vertices[2].x = mesh[i].vertices[2].x;
    triangles[i].vertices[2].y = mesh[i].vertices[2].y;
    triangles[i].vertices[2].z = mesh[i].vertices[2].z;
  }

  err = clEnqueueWriteBuffer(queue, input, CL_TRUE, 0, size, triangles, 0, NULL,
                             NULL);
  if (err != 0) {
    throw;
  }
}

void Engine::Render::SetArgument() {
  int err;

  err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
  if (err != 0) {
    throw;
  }

  struct s_camera c_camera;

  c_camera.height = camera.height;
  c_camera.width = camera.width;
  c_camera.fov = camera.fov;
  c_camera.focus = camera.focus;
  c_camera.position.x = camera.position.x;
  c_camera.position.y = camera.position.y;
  c_camera.position.z = camera.position.z;
  c_camera.number = mesh.size();
  c_camera.seed = rand();

  err = clSetKernelArg(kernel, 1, sizeof(struct s_camera), &c_camera);
  if (err != 0) {
    throw;
  }

  err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &output);
  if (err != 0) {
    throw;
  }
}
