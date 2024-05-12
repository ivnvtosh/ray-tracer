#ifndef SCENE_HPP
#define SCENE_HPP

#include <chrono>
#include <functional>

#include "Engine.hpp"
#include "Model.hpp"

class Scene {
 public:
  using TimeDuration = std::chrono::duration<double, std::milli>;

  Scene() = default;

  void Setup(Engine::Size);
  void SetupRender(Engine::Size);

  void Update();
  void Render(std::function<void(TimeDuration, double)> update);

  void KeyDown(Key);
  void KeyUp(Key);

  void LeftMouseDown(float x, float y);
  void LeftMouseDragged(float x, float y);
  void LeftMouseUp(float x, float y);

  void RightMouseDown(float x, float y);
  void RightMouseDragged(float x, float y);
  void RightMouseUp(float x, float y);

 private:
  Engine::Shader shader;
  Engine::RenderShader renderShader;

  Engine::Vector3 cameraMoveDirection = Engine::Vector3::Zero();
  Engine::Vector3 cameraAngle;
  float cameraSpeed = 0.002f;

  Engine::Vector2 cursorPosition;
};

#endif  // SCENE_HPP
