#ifndef SCENE_HPP
#define SCENE_HPP

#include <functional>

#include "Model.hpp"
#include "OBJImporter.hpp"
#include "PPMExporter.hpp"
#include "Shader.hpp"

namespace Engine {

class Time {
 public:
  static float deltaTime;
};

}  // namespace Engine

class Scene {
 public:
  Scene() = default;

  void Setup(int height, int width);

  void Update();

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

  Engine::Vector3 move;
  Engine::Vector3 angle;
  Engine::Vector3 positionCursor;
};

#endif  // SCENE_HPP
