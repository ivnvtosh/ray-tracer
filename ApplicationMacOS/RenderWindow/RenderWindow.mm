#include "RenderWindow.hpp"

#include "RenderViewController.hpp"

void RenderWindow::Create() noexcept {
  auto frame = NSMakeRect(0, 0, 512, 512);
  auto windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable;

  auto window = [[NSWindow alloc] initWithContentRect:frame
                                  styleMask:windowStyle
                                  backing:NSBackingStoreBuffered
                                  defer:NO];

  auto viewController = [[RenderViewController alloc] initWithNibName:nil bundle:nil];

  [window center];
  [window setTitlebarAppearsTransparent:YES];
  [window setContentView:viewController.view];
  [window makeKeyAndOrderFront:viewController];

  this->window = window;
  this->viewController = viewController;
}

void RenderWindow::Update() const noexcept {
  [(id)viewController selectOpenGLContext];
}

void RenderWindow::Close() const noexcept {
	[(id)window close];
}
