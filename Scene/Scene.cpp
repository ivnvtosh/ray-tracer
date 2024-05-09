#include "Scene.hpp"

#include <iostream>
#include <sstream>

namespace Engine {

float Time::deltaTime = 0.0f;

}  // namespace Engine

typedef float t_col[3];
typedef t_col t_mat[3];

void matrix_new(t_mat matrix);
void matrix_rotate(t_mat matrix, Engine::Vector3 angle);

void Scene::Setup(int height, int width) {
  shader = Engine::Shader(height, width);

  auto request = Engine::OBJRequest().Filepath("../../").Filename("untitled");

  auto mesh = Engine::OBJImporter().Import(request);

  shader.LoadMesh(mesh);

  shader.camera.height = height;
  shader.camera.width = width;
  shader.camera.fov = 45.0f;
  shader.camera.focus =
      width / 2 / std::tanf(shader.camera.fov / 2.0f * M_PI / 180.0f);
  shader.camera.position.x = -0.01f;
  shader.camera.position.y = 0.99f;
  shader.camera.position.z = 3.39f;
  shader.camera.rotation.x = 0.0f;
  shader.camera.rotation.y = 0.0f;
  shader.camera.rotation.z = 0.0f;
  move.x = 0.0f;
  move.y = 0.0f;
  move.z = 0.0f;
  angle.x = 0.0f;
  angle.y = -90.0f;
  angle.z = 90.0f;
}

void Scene::Update() {
  auto speed = 0.2f;

  matrix_new(shader.camera.matrix);
  matrix_rotate(shader.camera.matrix, shader.camera.rotation);

  auto mov = move;

  mov.x = shader.camera.matrix[0][0] * move.x +
          shader.camera.matrix[0][1] * move.y +
          shader.camera.matrix[0][2] * move.z;

  mov.y = shader.camera.matrix[1][0] * move.x +
          shader.camera.matrix[1][1] * move.y +
          shader.camera.matrix[1][2] * move.z;

  mov.z = shader.camera.matrix[2][0] * move.x +
          shader.camera.matrix[2][1] * move.y +
          shader.camera.matrix[2][2] * move.z;

  shader.camera.position.x += mov.x * speed * (Engine::Time::deltaTime / 100);
  shader.camera.position.y += mov.y * speed * (Engine::Time::deltaTime / 100);
  shader.camera.position.z += mov.z * speed * (Engine::Time::deltaTime / 100);

  shader.camera.rotation.x = angle.x;
  shader.camera.rotation.y = angle.y;
  shader.camera.rotation.z = angle.z;

  shader.UpdateState(0, 0, shader.camera.width, shader.camera.height);
  shader.UpdateShader();
}

void Scene::KeyDown(Key key) {
  switch (key) {
    case W:
      move.x = 1.0f;
      break;

    case S:
      move.x = -1.0f;
      break;

    case Space:
      move.z = -1.0f;
      break;

    case Z:
      move.z = 1.0f;
      break;

    case D:
      move.y = 1.0f;
      break;

    case A:
      move.y = -1.0f;
      break;

    default:
      break;
  }
}

void Scene::KeyUp(Key key) {
  switch (key) {
    case W:
    case S:
      move.x = 0.0f;
      break;

    case Space:
    case Z:
      move.z = 0.0f;
      break;

    case D:
    case A:
      move.y = 0.0f;
      break;

    default:
      break;
  }
}

void Scene::LeftMouseDown(float x, float y) {
  positionCursor.x = x;
  positionCursor.y = y;
}

void Scene::LeftMouseDragged(float x, float y) {
  auto speed = 0.1f;

  angle.x -= (x - positionCursor.x) * speed;
  angle.y -= (y - positionCursor.y) * speed;

  positionCursor.x = x;
  positionCursor.y = y;
}

void Scene::LeftMouseUp(float x, float y) {}

void Scene::RightMouseDown(float x, float y) {}

void Scene::RightMouseDragged(float x, float y) {}

void Scene::RightMouseUp(float x, float y) {}
