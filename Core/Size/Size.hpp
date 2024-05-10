#ifndef SIZE_HPP
#define SIZE_HPP

namespace Engine {

struct Size {
 public:
  int width;
  int height;

  Size() noexcept = default;
  Size(int width, int height) noexcept;
};

}  // namespace Engine

#endif  // SIZE_HPP
