#include "Shader.hpp"

#include <string.h>

#include <iostream>

#include "kernel_shared.h"

Engine::Shader::Shader(int width, int height)
    : width(width), height(height), data(new int[height * width]) {
  CreateTexture();
  CreateShaderProgram();
  UseProgram();

  camera.height = 512;
  camera.width = 512;

  GetDeviceId();
  CreateContext();
  CreateCommandQueue();
  CreateKernel();
  LoadMem();
  SetArgument();
}

Engine::Shader::~Shader() { delete[] data; }

void Engine::Shader::Update() {
  UpdateState();
  UpdateShader();
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

void Engine::Shader::CreateShaderProgram() {
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

void Engine::Shader::UpdateShader() const noexcept {
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

//
//
//

void Engine::Shader::GetDeviceId() {
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

void Engine::Shader::CreateContext() {
  int err;

  context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
  if (err != 0) {
    throw;
  }
}

void Engine::Shader::CreateCommandQueue() {
  int err;

  queue = clCreateCommandQueue(context, device, 0, &err);
  if (err != 0) {
    throw;
  }
}

void Engine::Shader::CreateKernel() {
  int err;

  auto program = CreateProgram();

  err = clBuildProgram(program, 1, &device, "-I ../Engine/Source", NULL, NULL);
  if (err != 0) {
    size_t size;
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL,
                          &size);
    auto log = new char[size];
    clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, size, log,
                          NULL);

    std::cout << log << std::endl;

    delete[] log;

    throw;
  }

  kernel = clCreateKernel(program, "render", &err);
  if (err != 0) {
    throw;
  }
}

cl_program Engine::Shader::CreateProgram() {
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

void Engine::Shader::ReadFilesToBuffer(char **program_buf,
                                       size_t *program_size) {
  program_buf[0] =
      ReadFileToBuffer("../Engine/Source/kernel.cl", &program_size[0]);
}

char *Engine::Shader::ReadFileToBuffer(const char *name, size_t *program_size) {
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

void Engine::Shader::LoadMem() {
  int err;
  auto bytes = camera.height * camera.width * sizeof(int);

  output = clCreateBuffer(context, CL_MEM_READ_WRITE, bytes, NULL, &err);

  if (err != 0) {
    throw;
  }
}

void Engine::Shader::SetArgument() {
  int err;

  // err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
  // if (err != 0) {
  //   throw;
  // }

  err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &output);
  if (err != 0) {
    throw;
  }
}

void Engine::Shader::UpdateState() const {
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

void Engine::Shader::LoadMesh(TriangleMesh mesh) {
  int err;

  this->mesh = mesh;
  auto size = mesh.size() * sizeof(struct s_triangle);
  input = clCreateBuffer(context, CL_MEM_READ_ONLY, size, NULL, &err);
  if (err != 0) {
    throw;
  }

  auto triangles =
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
  free(triangles);
  if (err != 0) {
    throw;
  }

  err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
  if (err != 0) {
    throw;
  }
}
