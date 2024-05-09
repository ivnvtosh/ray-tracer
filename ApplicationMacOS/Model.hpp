#ifndef MODEL_HPP
#define MODEL_HPP

#include <functional>

enum Key { W = 13, A = 0, S = 1, D = 2, Z = 6, Space = 49 };

struct Model {
 public:
  std::function<void(Key)> keyDown;
  std::function<void(Key)> keyUp;

  std::function<void(float, float)> leftMouseDown;
  std::function<void(float, float)> leftMouseDragged;
  std::function<void(float, float)> leftMouseUp;
  
  std::function<void(float, float)> rightMouseDown;
  std::function<void(float, float)> rightMouseDragged;
  std::function<void(float, float)> rightMouseUp;

  std::function<void(void)> update;

  std::function<void(void)> close;
};

#endif  // MODEL_HPP
