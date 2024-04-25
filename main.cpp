// #include "Logger.hpp"
// #include "Scene.hpp"

extern "C" void render();

int main(void) {
  // auto logger = Logger();
  // auto scene = Scene();

  // logger.TimeStartImport();
  // scene.Import();
  // logger.TimeEndImport();
  // logger.LogTimeDurationImport();

  // scene.render.Setup();

  // logger.TimeStartRender();
  // scene.Render();
  // logger.TimeEndRender();
  // logger.LogTimeDurationRender();

  // logger.TimeStartExport();
  // scene.Export();
  // logger.TimeEndExport();
  // logger.LogTimeDurationExport();

  render();
  return 0;
}
