#ifndef GENERAL_WINDOW_MODEL_HPP
#define GENERAL_WINDOW_MODEL_HPP

#include <functional>
#include <optional>

struct GeneralWindowModel {
 public:
  std::optional<std::function<void(void)>> render;
};

#endif  // GENERAL_WINDOW_MODEL_HPP
