#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Scene.hpp"

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
  void CreateScene();
  void Loop();

 private:
  t_app app;
  Scene scene;

  void WillCreateScene();
};

#endif  // APPLICATION_HPP
