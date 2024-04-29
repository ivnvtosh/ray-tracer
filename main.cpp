#include "Application.hpp"

int main(void) {
  auto application = Application();

  auto height = 512;
  auto width = 512;
  auto title = "Ray Tracer";
  application.CreateWindow(height, width, title);

  application.Loop();
  
  return 0;
}
