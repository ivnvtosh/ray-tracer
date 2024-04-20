#include "Scene.hpp"

Scene::Scene() {
  auto height = 512;
  auto width = 512;
  auto fov = 45.0f;

  render.camera.height = height;
  render.camera.width = width;
  render.camera.fov = fov;
  render.camera.focus = height / 2 / std::tanf(fov / 2.0f * M_PI / 180.0f);

  data = (int*)malloc(height * width * sizeof(int));
}

Scene::~Scene() { free(data); }

void Scene::Import() {
  auto request = Engine::OBJRequest()
                     .Filepath("../cornell-box-original/")
                     .Filename("cornell-box-original");

  render.mesh = Engine::OBJImporter().Import(request);
}

void Scene::Render() { render.Rendering(data); }

void Scene::Export() {
  auto request = Engine::PPMRequest()
                     .Filepath("../../")
                     .Filename("image")
                     .Height(render.camera.height)
                     .Width(render.camera.width)
                     .Data(data);

  Engine::PPMExporter().Export(request);
}
