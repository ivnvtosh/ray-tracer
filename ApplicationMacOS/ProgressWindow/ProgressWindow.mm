#include "ProgressWindow.hpp"

#include "ProgressViewController.hpp"

void ProgressWindow::Create(ProgressWindowModel model) noexcept {
  auto frame = NSMakeRect(0, 0, 384, 0);
  auto windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable;

  auto window = [[NSWindow alloc] initWithContentRect:frame
                                            styleMask:windowStyle
                                              backing:NSBackingStoreBuffered
                                                defer:NO];

  auto viewController =
      [[ProgressViewController alloc] initWithNibName:nil bundle:nil];

  [viewController setModel:model];

  [window center];
  [window setTitlebarAppearsTransparent:YES];
  [window setContentView:viewController.view];
  [window makeKeyAndOrderFront:viewController];

  this->window = window;
  this->viewController = viewController;
}

void ProgressWindow::Update(TimeDuration duration,
                            double value) const noexcept {
  [(id)viewController update:duration with:value];
}

void ProgressWindow::Done(TimeDuration total) const noexcept {
  [(id)viewController done];
}

void ProgressWindow::Close() const noexcept { [(id)window close]; }
