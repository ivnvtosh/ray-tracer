#include "Scene.hpp"

void Scene::KeyDown(Key key) {
  switch (key) {
    case W:
      cameraMoveDirection.x += +1.0f;
      break;

    case S:
      cameraMoveDirection.x += -1.0f;
      break;

    case Space:
      cameraMoveDirection.z += -1.0f;
      break;

    case Z:
      cameraMoveDirection.z += +1.0f;
      break;

    case D:
      cameraMoveDirection.y += +1.0f;
      break;

    case A:
      cameraMoveDirection.y += -1.0f;
      break;

    default:
      break;
  }
}

void Scene::KeyUp(Key key) {
  switch (key) {
    case W:
      cameraMoveDirection.x -= +1.0f;
      break;

    case S:
      cameraMoveDirection.x -= -1.0f;
      break;

    case Space:
      cameraMoveDirection.z -= -1.0f;
      break;

    case Z:
      cameraMoveDirection.z -= +1.0f;
      break;

    case D:
      cameraMoveDirection.y -= +1.0f;
      break;

    case A:
      cameraMoveDirection.y -= -1.0f;
      break;

    default:
      break;
  }
}

void Scene::LeftMouseDown(float x, float y) {
  cursorPosition.x = x;
  cursorPosition.y = y;
}

void Scene::LeftMouseDragged(float x, float y) {
  auto speed = 0.1f;

  cameraAngle.x += (cursorPosition.x - x) * speed;
  cameraAngle.y += (cursorPosition.y - y) * speed;

  cursorPosition.x = x;
  cursorPosition.y = y;
}

void Scene::LeftMouseUp(float x, float y) {
  (void)x;
  (void)y;
}

void Scene::RightMouseDown(float x, float y) {
  (void)x;
  (void)y;
}

void Scene::RightMouseDragged(float x, float y) {
  (void)x;
  (void)y;
}

void Scene::RightMouseUp(float x, float y) {
  (void)x;
  (void)y;
}
