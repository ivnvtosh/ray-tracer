#ifndef SCENE_HPP
#define SCENE_HPP

#include "OBJImporter.hpp"
#include "PPMExporter.hpp"
#include "Shader.hpp"

class Scene {
 public:
  Scene() = default;

  void Setup();
  void Update();

 private:
  Engine::Shader shader;
};

#endif  // SCENE_HPP
