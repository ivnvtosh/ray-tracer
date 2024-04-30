#ifndef SHADER_HPP
#define SHADER_HPP

#define GL_SILENCE_DEPRECATION

#include <OpenCL/cl.h>
#include <OpenGL/gl3.h>

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

class Shader {
 public:
  Camera camera;

  Shader() = default;
  Shader(int width, int height);
  ~Shader();

  void Update();

  void LoadMesh(TriangleMesh);

 private:
  TriangleMesh mesh;

  int width;
  int height;
  int *data;

  GLuint program;
  GLuint texture;
  GLuint vbuffer;

  GLint image_texture;
  GLint image_pos;
  GLint image_size;
  GLint image_winhalfsize;
  GLint image_position;

  void CreateTexture() noexcept;
  void CreateShaderProgram();
  GLuint CreateVertexShader() const;
  GLuint CreateFragmentShader() const;
  void UseProgram() noexcept;
  void UpdateShader() const noexcept;

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

  void UpdateState() const;
};

};  // namespace Engine

#endif  // SHADER_HPP
