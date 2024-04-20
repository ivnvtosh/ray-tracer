#include "OBJImporter.hpp"

int main(void) {
  auto request = Engine::OBJRequest()
                     .Filepath("../cornell-box-original/")
                     .Filename("cornell-box-original");

  auto mesh = Engine::OBJImporter().Import(request);

  return 0;
}
