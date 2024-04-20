#include "Logger.hpp"
#include "Scene.hpp"

int main(void) {
  auto logger = Logger();
  auto scene = Scene();

  logger.TimeStartImport();
  scene.Import();
  logger.TimeEndImport();
  logger.LogTimeDurationImport();

  logger.TimeStartRender();
  scene.Render();
  logger.TimeEndRender();
  logger.LogTimeDurationRender();

  logger.TimeStartExport();
  scene.Export();
  logger.TimeEndExport();
  logger.LogTimeDurationExport();

  return 0;
}
