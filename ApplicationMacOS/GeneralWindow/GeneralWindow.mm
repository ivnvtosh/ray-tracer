#include "GeneralWindow.hpp"

#include "GeneralViewController.hpp"

void GeneralWindow::Create(GeneralWindowModel model) noexcept {
  auto frame = NSMakeRect(0, 0, 0, 0);
  auto windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable;

  auto window = [[NSWindow alloc] initWithContentRect:frame
                                  styleMask:windowStyle
                                  backing:NSBackingStoreBuffered
                                  defer:NO];

  auto viewController = [[GeneralViewController alloc] initWithNibName:nil bundle:nil];

  [viewController setModel:model];

  [window center];
  [window setTitlebarAppearsTransparent:YES];
  [window setContentView:viewController.view];
  [window makeKeyAndOrderFront:viewController];

  this->window = window;
  this->viewController = viewController;
}

void GeneralWindow::Close() const noexcept { 
	[(id)window close]; 
}
