#include "PPMExporter.hpp"

#include <fstream>

using namespace Engine;

PPMRequest& PPMRequest::Filepath(std::string_view filepath) noexcept {
  this->filepath = filepath;
  return *this;
}

PPMRequest& PPMRequest::Filename(std::string_view filename) noexcept {
  this->filename = filename;
  return *this;
}

PPMRequest& PPMRequest::Height(int height) noexcept {
  this->height = height;
  return *this;
}

PPMRequest& PPMRequest::Width(int width) noexcept {
  this->width = width;
  return *this;
}

PPMRequest& PPMRequest::Data(const int* data) noexcept {
  this->data = data;
  return *this;
}

void PPMExporter::Export(PPMRequest request) const {
  std::ofstream stream(std::string(request.filepath) + "/" +
                       std::string(request.filename) + ".ppm");

  stream << "P3" << "\n";
  stream << request.width << " " << request.height << " " << "255" << "\n";

  int color;
  for (auto y = 0; y < request.height; y += 1) {
    for (auto x = 0; x < request.width; x += 1) {
      color = request.data[y * request.height + x];

      stream << (color >> 16 & 0xFF) << "\n"
             << (color >> 8 & 0xFF) << "\n"
             << (color & 0xFF) << "\n";
    }
  }
}
