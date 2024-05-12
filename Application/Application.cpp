#include "Application.hpp"

#include <chrono>
#include <iostream>
#include <thread>

t_app createApplication(int height, int width, const char *title);
void *createAnotherWindow(t_app *, int height, int width, const char *title);
void loop(t_app *app);
void select_glContext(t_app *app);
void progressIndicator(t_app app, double value);
void timer(t_app app, char *str);
void createWindowSettings(t_app *app);
void setModel(t_app app, Model model);

Application::Application() {}

void Application::CreateWindow(int height, int width, const char *title) {
  app = createApplication(height, width, title);

  auto model = Model();

  model.keyDown = [this](Key key) { KeyDown(key); };
  model.keyUp = [this](Key key) { KeyUp(key); };

  model.leftMouseDown = [this](float x, float y) { LeftMouseDown(x, y); };
  model.leftMouseDragged = [this](float x, float y) { LeftMouseDragged(x, y); };
  model.leftMouseUp = [this](float x, float y) { LeftMouseUp(x, y); };

  model.rightMouseDown = [this](float x, float y) { RightMouseDown(x, y); };
  model.rightMouseDragged = [this](float x, float y) {
    RightMouseDragged(x, y);
  };
  model.rightMouseUp = [this](float x, float y) { RightMouseUp(x, y); };

  // // model.update = [this]() { Update(); };

  setModel(app, model);

  // // createAnotherWindow(&app, 48, 384, "Calculate");
  // // createWindowSettings(&app);

  auto generalWindowModel = GeneralWindowModel();

  generalWindowModel.render = [this]() {
    renderWindow = RenderWindow();
    renderWindow.Create();
    auto progressWindowModel = ProgressWindowModel();
    progressWindow = ProgressWindow();
    progressWindow.Create(progressWindowModel);

    std::thread thread(&Application::Render, this);
    thread.detach();
  };

  GeneralWindow().Create(generalWindowModel);
}

void Application::CreateScene() {
  std::thread thread(&Application::WillCreateScene, this);
  thread.detach();
}

void Application::WillCreateScene() {
  select_glContext(&app);
  scene.Setup(Engine::Size(app.width, app.height));
  while (true) {
    auto start = std::chrono::high_resolution_clock::now();
    Update();
    glFlush();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

    Engine::Time::deltaTime = ms.count();
  }
}

void Application::Render() {
  renderWindow.Update();
  scene.SetupRender(Engine::Size(app.width, app.height));

  auto start = std::chrono::high_resolution_clock::now();

  scene.Render([this](Scene::TimeDuration duration, double value) {
    progressWindow.Update(duration, value);
  });

  auto end = std::chrono::high_resolution_clock::now();

  auto duration = std::chrono::duration<double, std::milli>(end - start);
  progressWindow.Done(duration);
}

void Application::Loop() { loop(&app); }

void Application::KeyDown(Key key) {
  // printf("Key Down: %d\n", key);
  scene.KeyDown(key);
}

void Application::KeyUp(Key key) {
  // printf("Key Up: %d\n", key);
  scene.KeyUp(key);
}

void Application::LeftMouseDown(float x, float y) {
  // printf("Left Mouse Down x: %f, y: %f\n", x, y);
  scene.LeftMouseDown(x, y);
}

void Application::LeftMouseDragged(float x, float y) {
  // printf("Left Mouse Dragged x: %f, y: %f\n", x, y);
  scene.LeftMouseDragged(x, y);
}

void Application::LeftMouseUp(float x, float y) {
  // printf("Left Mouse Up x: %f, y: %f\n", x, y);
  scene.LeftMouseUp(x, y);
}

void Application::RightMouseDown(float x, float y) {
  // printf("Right Mouse Down x: %f, y: %f\n", x, y);
  scene.RightMouseDown(x, y);
}

void Application::RightMouseDragged(float x, float y) {
  // printf("Right Mouse Dragged x: %f, y: %f\n", x, y);
  scene.RightMouseDragged(x, y);
}

void Application::RightMouseUp(float x, float y) {
  // printf("Right Mouse Up x: %f, y: %f\n", x, y);
  scene.RightMouseUp(x, y);
}

void Application::Update() { scene.Update(); }
