#include "Scene.hpp"

#include <iostream>

void Scene::Setup(int height, int width) {
  shader = Engine::Shader(height, width);

  auto request = Engine::OBJRequest()
                     .Filepath("../cornell-box-original/")
                     .Filename("cornell-box-original");

  auto mesh = Engine::OBJImporter().Import(request);

  shader.LoadMesh(mesh);

  shader.camera.height = height;
  shader.camera.width = width;
  shader.camera.fov = 45.0f;
  shader.camera.focus =
      height / 2 / std::tanf(shader.camera.fov / 2.0f * M_PI / 180.0f);
  shader.camera.position.x = -0.01f;
  shader.camera.position.y = 0.99f;
  shader.camera.position.z = 3.39f;
}

void Scene::Update(std::function<void(double)> progressIndicatorUpdate) {
  auto block = 64;

  auto m = shader.camera.height / block;
  auto n = shader.camera.width / block;
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
      shader.UpdateState(x, y, block, block);
      shader.UpdateShader();
      glFlush();
      timeEnd = std::chrono::high_resolution_clock::now();
      LogTimeDuration();
      progressIndicatorUpdate((float)count / ((float)m * (float)n) * 100.0f);
      count += 1;
    }

    for (auto i = top; i < bottom; i += 1) {
      timeStart = std::chrono::high_resolution_clock::now();
      y = i * block;
      x = right * block;
      shader.UpdateState(x, y, block, block);
      shader.UpdateShader();
      glFlush();
      timeEnd = std::chrono::high_resolution_clock::now();
      LogTimeDuration();
      progressIndicatorUpdate((float)count / ((float)m * (float)n) * 100.0f);
      count += 1;
    }

    right++;

    for (auto i = right - 1; i > left; i -= 1) {
      timeStart = std::chrono::high_resolution_clock::now();
      y = bottom * block;
      x = i * block;
      shader.UpdateState(x, y, block, block);
      shader.UpdateShader();
      glFlush();
      timeEnd = std::chrono::high_resolution_clock::now();
      LogTimeDuration();
      progressIndicatorUpdate((float)count / ((float)m * (float)n) * 100.0f);
      count += 1;
    }

    bottom++;

    for (auto i = bottom - 1; i > top; i -= 1) {
      timeStart = std::chrono::high_resolution_clock::now();
      y = i * block;
      x = left * block;
      shader.UpdateState(x, y, block, block);
      shader.UpdateShader();
      glFlush();
      timeEnd = std::chrono::high_resolution_clock::now();
      LogTimeDuration();
      progressIndicatorUpdate((float)count / ((float)m * (float)n) * 100.0f);
      count += 1;
    }

    left--;
    top--;
  }

  auto request = Engine::PPMRequest()
                     .Filepath("../../")
                     .Filename("image")
                     .Height(shader.camera.height)
                     .Width(shader.camera.width)
                     .Data(shader.data);

  Engine::PPMExporter().Export(request);
}

void Scene::LogTimeDuration() {
  using namespace std::chrono;

  duration<double, std::milli> duration(timeEnd - timeStart);

  auto h = duration_cast<hours>(duration);
  auto m = duration_cast<minutes>(duration - h);
  auto s = duration_cast<seconds>(duration - h - m);
  auto ms = duration_cast<milliseconds>(duration - h - m - s);

  std::cout << "Block" << ": ";

  auto empty = true;

  if (auto hours = h.count()) {
    std::cout << hours << "h ";
    empty = false;
  }

  if (auto minutes = m.count()) {
    std::cout << minutes << "m ";
    empty = false;
  }

  if (auto seconds = s.count()) {
    std::cout << seconds << "s ";
    empty = false;
  }

  if (auto milliseconds = ms.count()) {
    std::cout << milliseconds << "ms";
    empty = false;
  }

  if (empty) {
    std::cout << "0ms";
  }

  std::cout << std::endl;
}
