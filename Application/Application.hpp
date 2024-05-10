#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Scene.hpp"

struct s_app {
  void *application;
  void *window;
  void *view;
  void *win;
  void *progressIndicator;
  void *timer;

  void *windowSettings;

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

  void KeyDown(Key);
  void KeyUp(Key);

  void LeftMouseDown(float x, float y);
  void LeftMouseDragged(float x, float y);
  void LeftMouseUp(float x, float y);

  void RightMouseDown(float x, float y);
  void RightMouseDragged(float x, float y);
  void RightMouseUp(float x, float y);

  void Update();
};

#endif  // APPLICATION_HPP
