#include "Scene.hpp"

void Scene::Setup() {
  auto height = 512;
  auto width = 512;

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

void Scene::Update() { shader.Update(); }