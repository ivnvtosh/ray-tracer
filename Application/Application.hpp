#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Shader.hpp"

struct s_app {
  void *application;
  void *window;
  void *view;

  int width;
  int height;
};

typedef struct s_app t_app;

class Application {
 public:
  Application();
  void CreateWindow(int height, int width, const char *title);
  void Loop();
  void Update();

 private:
  t_app app;
  Engine::Shader shader;
  int *data;
};

#endif  // APPLICATION_HPP
