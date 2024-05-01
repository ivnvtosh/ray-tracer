#ifndef SCENE_HPP
#define SCENE_HPP

#include <chrono>

#include "OBJImporter.hpp"
#include "PPMExporter.hpp"
#include "Shader.hpp"

class Scene {
 public:
  Scene() = default;

  void Setup(int height, int width);
  void Update();

 private:
  Engine::Shader shader;

  using TimePoint = std::chrono::high_resolution_clock::time_point;
  TimePoint timeStart;
  TimePoint timeEnd;

  void LogTimeDuration();
};

#endif  // SCENE_HPP
