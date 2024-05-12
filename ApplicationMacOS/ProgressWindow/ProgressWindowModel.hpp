#ifndef PROGRESS_WINDOW_MODEL_HPP
#define PROGRESS_WINDOW_MODEL_HPP

#include <functional>
#include <optional>

struct ProgressWindowModel {
 public:
  std::optional<std::function<void(void)>> pause;
  std::optional<std::function<void(void)>> play;
  std::optional<std::function<void(void)>> cancel;
};

#endif  // PROGRESS_WINDOW_MODEL_HPP
