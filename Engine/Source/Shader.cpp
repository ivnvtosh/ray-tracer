#include "Shader.hpp"

#include <string.h>

Engine::Shader::Shader(int width, int height, int *data)
    : width(width), height(height), data(data) {
  CreateTexture();
  CreateProgram();
  UseProgram();
}

void Engine::Shader::Update() {
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA,
               GL_UNSIGNED_BYTE, data);

  glUseProgram(program);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glUniform1i(image_texture, 0);
  glUniform2f(image_winhalfsize, width / 2, height / 2);
  glUniform2f(image_pos, 0, height);
  glUniform2f(image_size, width, -height);

  glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
  glEnableVertexAttribArray(0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
  glBlendEquation(GL_FUNC_ADD);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableVertexAttribArray(0);
}

void Engine::Shader::CreateTexture() noexcept {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA,
               GL_UNSIGNED_BYTE, data);

  GLfloat vertexes[8];

  vertexes[0] = 0.0f;
  vertexes[1] = 0.0f;
  vertexes[2] = static_cast<GLfloat>(width);
  vertexes[3] = 0.0f;
  vertexes[4] = static_cast<GLfloat>(width);
  vertexes[5] = static_cast<GLfloat>(-height);
  vertexes[6] = 0.0f;
  vertexes[7] = static_cast<GLfloat>(-height);

  glGenBuffers(1, &vbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_DYNAMIC_DRAW);
}

void Engine::Shader::CreateProgram() {
  auto vertexShader = CreateVertexShader();
  auto fragmentShader = CreateFragmentShader();

  program = glCreateProgram();

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    throw;
  }

  glFlush();
}

void Engine::Shader::UseProgram() noexcept {
  glUseProgram(program);

  image_texture = glGetUniformLocation(program, "texture");
  image_pos = glGetUniformLocation(program, "imagepos");
  image_size = glGetUniformLocation(program, "imagesize");
  image_winhalfsize = glGetUniformLocation(program, "winhalfsize");
  image_position = glGetAttribLocation(program, "position");

  glFlush();
}

GLuint Engine::Shader::CreateVertexShader() const {
  auto source =
      "#version 110\n"
      "attribute vec2 position;"
      "uniform vec2 winhalfsize;"
      "uniform vec2 imagepos;"
      "uniform vec2 imagesize;"
      "varying vec2 texcoord;"
      "void main()"
      "{"
      "    texcoord = position / imagesize;"
      "    vec2 pos = position - winhalfsize + imagepos;"
      "    pos = pos / winhalfsize;"
      "    gl_Position = vec4(pos, 0.0, 1.0);"
      "}";

  auto length = static_cast<int>(strlen(source));
  auto shader = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(shader, 1, static_cast<const GLchar **>(&source), &length);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    throw;
  }

  return shader;
}

GLuint Engine::Shader::CreateFragmentShader() const {
  auto source =
      "#version 110 \n"
      "uniform sampler2D texture;"
      "varying vec2 texcoord;"
      "void main()"
      "{"
      "    gl_FragColor = texture2D(texture, texcoord);"
      "}";

  auto length = static_cast<int>(strlen(source));
  auto shader = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(shader, 1, static_cast<const GLchar **>(&source), &length);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    throw;
  }

  return shader;
}
