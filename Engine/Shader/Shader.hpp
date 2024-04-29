#ifndef SHADER_HPP
#define SHADER_HPP

#define GL_SILENCE_DEPRECATION

#include <OpenGL/gl3.h>

namespace Engine {
class Shader {
 public:
  Shader() = default;
  Shader(int width, int height, int *data);

  void Update();

 private:
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

  void CreateProgram();
  GLuint CreateVertexShader() const;
  GLuint CreateFragmentShader() const;

  void UseProgram() noexcept;
};

};  // namespace Engine

#endif  // SHADER_HPP
