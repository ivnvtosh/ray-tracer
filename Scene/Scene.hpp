#ifndef SCENE_HPP
#define SCENE_HPP

#include <chrono>
#include <functional>

#include "OBJImporter.hpp"
#include "PPMExporter.hpp"
#include "Shader.hpp"

class Scene {
 public:
  Scene() = default;

  void Setup(int height, int width);
  void Update(std::function<void(double)>, std::function<void(char*)>);

 private:
  Engine::Shader shader;

  using TimePoint = std::chrono::high_resolution_clock::time_point;
  TimePoint timeStart;
  TimePoint timeEnd;
  std::vector<std::chrono::duration<double, std::milli>> durations;
  std::string LogTimeDuration(int);
};

#endif  // SCENE_HPP
