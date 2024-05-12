#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

class RenderWindow {
 public:
  void Create() noexcept;
  void Update() const noexcept;
  void Close() const noexcept;

 private:
  void *window;
  void *viewController;
};

#endif  // RENDER_WINDOW_HPP
