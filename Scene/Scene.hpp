#ifndef SCENE_HPP
#define SCENE_HPP

#include "OBJImporter.hpp"
#include "PPMExporter.hpp"
#include "Render.hpp"

class Scene {
 public:
  Engine::Render render;
  Engine::TriangleMesh mesh;

  int* data;

  Scene(int* data);
  ~Scene();

  void Import();
  void Render();
  void Export();
};

#endif  // SCENE_HPP
