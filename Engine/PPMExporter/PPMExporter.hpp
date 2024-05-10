#ifndef PPMEXPORTER_HPP
#define PPMEXPORTER_HPP

#include <string_view>

namespace Engine {

struct PPMRequest {
 public:
  std::string_view filepath;
  std::string_view filename;
  int height;
  int width;
  const int* data;

  PPMRequest& Filepath(std::string_view filepath) noexcept;
  PPMRequest& Filename(std::string_view filename) noexcept;
  PPMRequest& Height(int height) noexcept;
  PPMRequest& Width(int width) noexcept;
  PPMRequest& Data(const int* data) noexcept;
};

class PPMExporter {
 public:
  void Export(PPMRequest) const;
};

}  // namespace Engine

#endif  // PPMEXPORTER_HPP
