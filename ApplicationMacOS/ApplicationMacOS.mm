#import "ApplicationMacOS.hpp"

NSWindow* createWindow(int height, int width, const char *title);
OpenGLView* createView(int height, int width);

#include <iostream>

struct s_app {
  void *application;
  void *window;
  void *view;
  void *win;
  void *progressIndicator;
  void *timer;
  
  void *windowSettings;

  int width;
  int height;
};

typedef struct s_app t_app;

@implementation OpenGLView

- (void) selectGLContext
{
  if ([NSOpenGLContext currentContext] != [self openGLContext])
  {
    [[self openGLContext] makeCurrentContext];
  }
}

@end

t_app createApplication(int height, int width, const char *title) {
  t_app app;
  
  NSApplication* application = [NSApplication sharedApplication];
  NSWindow* window = createWindow(height, width, title);
  OpenGLView* view = createView(height, width);

  [window setContentView:view];
  [window makeKeyAndOrderFront:view];

  app.application = application;
  app.window = window;
  app.view = view;
  app.height = height;
  app.width = width;
  
  // ProcessSerialNumber psn = { 0, kCurrentProcess };
  // TransformProcessType(&psn, kProcessTransformToForegroundApplication);

  return app;
}

NSWindow* createWindow(int height, int width, const char *title) {
  NSRect frame = NSMakeRect(0, 0, width, height);
  
  NSUInteger windowStyle = NSWindowStyleMaskTitled 
    | NSWindowStyleMaskClosable 
    | NSWindowStyleMaskMiniaturizable;

  NSWindow* window = [[MainWindow alloc] initWithContentRect:frame
                       		             styleMask:windowStyle
                                   	   backing:NSBackingStoreBuffered
                                  	   defer:NO];

  NSString* string = [NSString stringWithCString:title encoding:NSASCIIStringEncoding];
  
  [window setTitle:string];

  return window;
}

OpenGLView* createView(int height, int width) {
  NSRect frame = NSMakeRect(0, 0, width, height);
  
  NSOpenGLPixelFormatAttribute pfa_attrs[] = {
    NSOpenGLPFADepthSize, 
    32,
    NSOpenGLPFAOpenGLProfile, 
    NSOpenGLProfileVersionLegacy,
    0
  };

  NSOpenGLPixelFormat* pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:pfa_attrs];

  OpenGLView* view = [[OpenGLView alloc] initWithFrame:frame 
  										 pixelFormat:pixFmt];

  [[view openGLContext] makeCurrentContext];
  [[view openGLContext] setView:view];

  [view prepareOpenGL];
  [view setNextKeyView:view];

  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();

  [pixFmt release];

  return view;
}

void select_glContext(t_app* app)
{
  [(id)app->view selectGLContext];
}

void loop(t_app* app)
{
  [NSApp run];
}

void createWindowSettings(t_app* app) {
  NSScreen *mainScreen = [NSScreen mainScreen];
  NSRect screenFrame = [mainScreen frame];
  CGFloat x = screenFrame.size.width / 2 - 256 / 2 + 256 ;
  CGFloat y = screenFrame.size.height / 2 - 512 / 2;

  NSRect frame = NSMakeRect(x, y, 256, 512);
  
  NSUInteger windowStyle = NSWindowStyleMaskTitled 
    | NSWindowStyleMaskClosable 
    | NSWindowStyleMaskMiniaturizable;

  NSWindow* window = [[NSWindow alloc] initWithContentRect:frame
                       		             styleMask:windowStyle
                                   	   backing:NSBackingStoreBuffered
                                  	   defer:NO];

  [window setTitle:@"Settings"];
  [window makeKeyAndOrderFront:nil];

  app->windowSettings = window;

  NSTextField *numberOfRaysLabel = [[NSTextField alloc] initWithFrame:NSMakeRect(16, 100, 200, 22)];

  [[window contentView] addSubview:numberOfRaysLabel];

  numberOfRaysLabel.bezeled = NO;
  numberOfRaysLabel.editable = NO;
  numberOfRaysLabel.drawsBackground = NO;

  numberOfRaysLabel.textColor = [NSColor grayColor];
 [numberOfRaysLabel setStringValue:@"rays"];

  NSTextField *numberOfRaysTextField = [[NSTextField alloc] initWithFrame:NSMakeRect(48, 100, 192, 22)];

  [[window contentView] addSubview:numberOfRaysTextField];

  // timer.bezeled = NO;
  // timer.editable = NO;
  // timer.drawsBackground = NO;

  // timer.textColor = [NSColor grayColor];
}

void setModel(t_app app, Model model) {
  
  MainWindow* window = (id)app.window;

  window.model = model;
}
