#include "MainWindow.hpp"

@implementation MainWindow

- (void) keyDown:(NSEvent *)event
{
  if ([event isARepeat]) {
    return;
  }

  auto key = static_cast<Key>([event keyCode]);

  _model.keyDown(key);
}

- (void) keyUp:(NSEvent *)event
{
  if ([event isARepeat]) {
    return;
  }

  auto key = static_cast<Key>([event keyCode]);

  _model.keyUp(key);
}

- (void) mouseDown:(NSEvent *)event
{
  auto point = [event locationInWindow];

  _model.leftMouseDown(point.x, point.y);
}

- (void) mouseDragged:(NSEvent *)event
{
  auto point = [event locationInWindow];

  _model.leftMouseDragged(point.x, point.y);
}

- (void) mouseUp:(NSEvent *)event
{
  auto point = [event locationInWindow];

  _model.leftMouseUp(point.x, point.y);
}

- (void) rightMouseDown:(NSEvent *)event
{
  auto point = [event locationInWindow];

  _model.rightMouseDown(point.x, point.y);
}

- (void) rightMouseDragged:(NSEvent *)event
{
  auto point = [event locationInWindow];

  _model.rightMouseDragged(point.x, point.y);
}

- (void) rightMouseUp:(NSEvent *)event
{
  auto point = [event locationInWindow];

  _model.rightMouseUp(point.x, point.y);
}

- (void) closeNotification:(NSNotification *)note
{
  _model.close();
}

@end
