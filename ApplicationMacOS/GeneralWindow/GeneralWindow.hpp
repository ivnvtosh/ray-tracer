#ifndef GENERAL_WINDOW_HPP
#define GENERAL_WINDOW_HPP

#include "GeneralWindowModel.hpp"

class GeneralWindow {
 public:
  void Create(GeneralWindowModel) noexcept;
  void Close() const noexcept;

 private:
  void *window;
  void *viewController;
};

#endif  // GENERAL_WINDOW_HPP
