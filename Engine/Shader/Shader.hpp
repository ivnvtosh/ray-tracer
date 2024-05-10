#ifndef SHADER_HPP
#define SHADER_HPP

#define GL_SILENCE_DEPRECATION

#include <OpenCL/cl.h>
#include <OpenGL/gl3.h>

#include "Camera.hpp"
#include "Size.hpp"
#include "TriangleMesh.hpp"

namespace Engine {

class Shader {
 public:
  Camera camera;
  int *data;

  Shader() = default;
  Shader(Engine::Size);
  ~Shader();

  void Update();
  void UpdateShader() const noexcept;
  void UpdateState(int x, int y, int ox, int oy) const;

  void LoadMesh(TriangleMesh);

 private:
  TriangleMesh mesh;

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

};  // namespace Engine

#endif  // SHADER_HPP
