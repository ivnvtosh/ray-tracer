#include "PPMExporter.hpp"

int main(void) {
  int red = 255 << 16 | 0 << 8 | 0;
  int green = 0 << 16 | 255 << 8 | 0;
  int blue = 0 << 16 | 0 << 8 | 255;
  int white = 255 << 16 | 255 << 8 | 255;

  int data[4] = {red, green, blue, white};

  auto request = Engine::PPMRequest()
                     .Filepath("../../")
                     .Filename("image")
                     .Height(2)
                     .Width(2)
                     .Data(data);

  Engine::PPMExporter().Export(request);

  return 0;
}
