#include "Scene.hpp"

void Scene::Setup(int height, int width) {
  auto request = Engine::OBJRequest().Filepath("../../").Filename("untitled");
  auto mesh = Engine::OBJImporter().Import(request);

  shader = Engine::Shader(height, width);

  shader.LoadMesh(mesh);

  shader.camera.SetFov(45.0f);
  shader.camera.position = Engine::Vector3(-0.01f, 0.99f, 3.39f);
  shader.camera.Rotate(Engine::Vector3(0.0f, -90.0f, 90.0f));

  cameraAngle = shader.camera.rotation;
}

void Scene::Update() {
  auto move =
      Engine::Time::deltaTime * cameraSpeed * cameraMoveDirection.Normalized();

  shader.camera.Rotate(cameraAngle);
  shader.camera.AddPositionByDirection(move);

  shader.Update();
}
