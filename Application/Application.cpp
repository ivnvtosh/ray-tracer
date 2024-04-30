#include "Application.hpp"

extern "C" {
t_app createApplication(int height, int width, const char *title);
void loop(t_app *app);
void select_glContext(t_app *app);
}

Application::Application() {}

void Application::CreateWindow(int height, int width, const char *title) {
  app = createApplication(height, width, title);
  select_glContext(&app);
}

void Application::CreateScene() {
  scene.Setup();
  scene.Update();
  glFlush();
}

void Application::Loop() { loop(&app); }
