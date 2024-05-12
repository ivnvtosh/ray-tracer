#include "Scene.hpp"

void Scene::Setup(Engine::Size size) {
  auto request = Engine::OBJRequest().Filepath("../../").Filename("untitled");
  auto mesh = Engine::OBJImporter().Import(request);

  shader = Engine::Shader(size);

  shader.LoadMesh(mesh);

  shader.camera.SetFov(45.0f);
  shader.camera.position = Engine::Vector3(-0.01f, 0.99f, 3.39f);
  shader.camera.Rotate(Engine::Vector3(0.0f, -90.0f, 90.0f));

  cameraAngle = shader.camera.rotation;
}

void Scene::SetupRender(Engine::Size size) {
  auto request = Engine::OBJRequest().Filepath("../../").Filename("untitled");
  auto mesh = Engine::OBJImporter().Import(request);

  renderShader = Engine::RenderShader(size);

  renderShader.LoadMesh(mesh);

  renderShader.camera = shader.camera;
}

void Scene::Update() {
  auto move =
      Engine::Time::deltaTime * cameraSpeed * cameraMoveDirection.Normalized();

  shader.camera.Rotate(cameraAngle);
  shader.camera.AddPositionByDirection(move);

  shader.Update();
}

void Scene::Render(std::function<void(TimeDuration, double)> update) {
  using TimePoint = std::chrono::high_resolution_clock::time_point;
  TimePoint timeStart;
  TimePoint timeEnd;

  renderShader.camera = shader.camera;

  auto block = 64;

  auto m = renderShader.camera.size.height / block;
  auto n = renderShader.camera.size.width / block;
  auto top = (m - 1) / 2;
  auto bottom = m / 2;
  auto left = (n - 1) / 2;
  auto right = n / 2;

  int x;
  int y;
  auto count = 0;

  while (top >= 0 && bottom < m && left >= 0 && right < n) {
    for (auto i = left; i < right; i += 1) {
      timeStart = std::chrono::high_resolution_clock::now();
      y = top * block;
      x = i * block;
      renderShader.UpdateState(x, y, block, block);
      renderShader.UpdateRenderShader();
      glFlush();
      timeEnd = std::chrono::high_resolution_clock::now();
      auto duration = (timeEnd - timeStart) * ((m * n) - count);
      auto value = (double)count / ((double)m * (double)n) * 100.0f;
      update(duration, value);
      count += 1;
    }

    for (auto i = top; i < bottom; i += 1) {
      timeStart = std::chrono::high_resolution_clock::now();
      y = i * block;
      x = right * block;
      renderShader.UpdateState(x, y, block, block);
      renderShader.UpdateRenderShader();
      glFlush();
      timeEnd = std::chrono::high_resolution_clock::now();
      auto duration = (timeEnd - timeStart) * ((m * n) - count);
      auto value = (double)count / ((double)m * (double)n) * 100.0f;
      update(duration, value);
      count += 1;
    }

    right++;

    for (auto i = right - 1; i > left; i -= 1) {
      timeStart = std::chrono::high_resolution_clock::now();
      y = bottom * block;
      x = i * block;
      renderShader.UpdateState(x, y, block, block);
      renderShader.UpdateRenderShader();
      glFlush();
      timeEnd = std::chrono::high_resolution_clock::now();
      auto duration = (timeEnd - timeStart) * ((m * n) - count);
      auto value = (double)count / ((double)m * (double)n) * 100.0f;
      update(duration, value);
      count += 1;
    }

    bottom++;

    for (auto i = bottom - 1; i > top; i -= 1) {
      timeStart = std::chrono::high_resolution_clock::now();
      y = i * block;
      x = left * block;
      renderShader.UpdateState(x, y, block, block);
      renderShader.UpdateRenderShader();
      glFlush();
      timeEnd = std::chrono::high_resolution_clock::now();
      auto duration = (timeEnd - timeStart) * ((m * n) - count);
      auto value = (double)count / ((double)m * (double)n) * 100.0f;
      update(duration, value);
      count += 1;
    }

    left--;
    top--;
  }
}
