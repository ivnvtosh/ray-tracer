#ifndef PROGRESS_WINDOW_HPP
#define PROGRESS_WINDOW_HPP

#include <chrono>

#include "ProgressWindowModel.hpp"

class ProgressWindow {
 public:
  using TimeDuration = std::chrono::duration<double, std::milli>;

  void Create(ProgressWindowModel) noexcept;
  void Update(TimeDuration, double progress) const noexcept;
  void Done(TimeDuration total) const noexcept;
  void Close() const noexcept;

 private:
  void *window;
  void *viewController;
};

#endif  // PROGRESS_WINDOW_HPP
