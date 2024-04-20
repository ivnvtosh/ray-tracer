#include "Scene.hpp"

int main(void) {
  auto scene = Scene();

  scene.Import();
  scene.Render();
  scene.Export();

  return 0;
}
