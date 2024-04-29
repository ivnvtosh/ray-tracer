#include "Application.hpp"

#include <stdlib.h>

#include "Logger.hpp"
#include "Scene.hpp"

extern "C" {
t_app createApplication(int height, int width, const char *title);
void loop(t_app *app);
void select_glContext(t_app *app);
}

Application::Application() {}

void Application::CreateWindow(int height, int width, const char *title) {
  app = createApplication(height, width, title);
  data = (int *)malloc(height * width * sizeof(int));
  select_glContext(&app);
  shader = Engine::Shader(height, width, data);

  auto logger = Logger();
  auto scene = Scene(data);

  logger.TimeStartImport();
  scene.Import();
  logger.TimeEndImport();
  logger.LogTimeDurationImport();

  scene.render.Setup();

  logger.TimeStartRender();
  scene.Render();
  logger.TimeEndRender();
  logger.LogTimeDurationRender();

  logger.TimeStartExport();
  scene.Export();
  logger.TimeEndExport();
  logger.LogTimeDurationExport();

  select_glContext(&app);
  shader.Update();
  glFlush();
}

void Application::Loop() { loop(&app); }

void Application::Update() { shader.Update(); }
