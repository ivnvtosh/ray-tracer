#include "Application.hpp"

#include <thread>

extern "C" {
t_app createApplication(int height, int width, const char *title);
void *createAnotherWindow(t_app *, int height, int width, const char *title);
void loop(t_app *app);
void select_glContext(t_app *app);
void progressIndicator(t_app app, double value);
void timer(t_app app, char *str);
void createWindowSettings(t_app *app);
}

Application::Application() {}

void Application::CreateWindow(int height, int width, const char *title) {
  app = createApplication(height, width, title);

  createAnotherWindow(&app, 48, 384, "Calculate");
  createWindowSettings(&app);
}

void Application::CreateScene() {
  std::thread thread(&Application::WillCreateScene, this);
  thread.detach();
}

void Application::WillCreateScene() {
  select_glContext(&app);
  scene.Setup(app.height, app.width);
  scene.Update([this](double a) { progressIndicator(app, a); },
               [this](char *str) { timer(app, str); });
}

void Application::Loop() { loop(&app); }
