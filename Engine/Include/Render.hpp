#ifndef RENDER_HPP
#define RENDER_HPP

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl>
#endif

#include "TriangleMesh.hpp"

namespace Engine {

struct Camera {
 public:
  int height;
  int width;
  float fov;
  float focus;
  Vector3 position;
};

class Render {
 public:
  Camera camera;
  TriangleMesh mesh;

  void Setup();
  void Rendering(int *data);

 private:
  cl_platform_id platform;
  cl_device_id device;
  cl_context context;
  cl_command_queue queue;
  cl_kernel kernel;

  cl_mem input;
  cl_mem output;

  void GetDeviceId();
  void CreateContext();
  void CreateCommandQueue();
  void CreateKernel();
  cl_program CreateProgram();
  void ReadFilesToBuffer(char **program_buf, size_t *program_size);
  char *ReadFileToBuffer(const char *name, size_t *program_size);
  void LoadMem();
  void SetArgument();
};

}  // namespace Engine

#endif  // RENDER_HPP
