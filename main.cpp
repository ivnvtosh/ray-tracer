#include "Logger.hpp"
#include "Scene.hpp"

extern "C" void render(int *data, int (*funct)());
extern "C" void loop();

auto logger = Logger();
auto scene = Scene();

int update() {
  scene.Render();
  return 0;
}

int main(void) {
  logger.TimeStartImport();
  scene.Import();
  logger.TimeEndImport();
  logger.LogTimeDurationImport();

  scene.render.Setup();

  logger.TimeStartRender();
  scene.Render();
  logger.TimeEndRender();
  logger.LogTimeDurationRender();

  logger.TimeStartExport();
  scene.Export();
  logger.TimeEndExport();
  logger.LogTimeDurationExport();

  render(scene.data, update);

  return 0;
}
