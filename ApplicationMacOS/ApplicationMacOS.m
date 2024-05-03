#import "ApplicationMacOS.h"

NSWindow* createWindow(int height, int width, const char *title);
OpenGLView* createView(int height, int width);

struct s_app {
  void *application;
  void *window;
  void *view;
  void *win;
  void *progressIndicator;
  void *timer;

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
  
  return app;
}

NSWindow* createWindow(int height, int width, const char *title) {
  NSRect frame = NSMakeRect(0, 0, width, height);
  
  NSUInteger windowStyle = NSWindowStyleMaskTitled 
    | NSWindowStyleMaskClosable 
    | NSWindowStyleMaskMiniaturizable;

  NSWindow* window = [[NSWindow alloc] initWithContentRect:frame
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

NSWindow* createAnotherWindow(t_app *app, int height, int width, const char *title) {
  NSScreen *mainScreen = [NSScreen mainScreen];
  NSRect screenFrame = [mainScreen frame];
  CGFloat x = screenFrame.size.width / 2 - width / 2;
  CGFloat y = screenFrame.size.height / 2 - height / 2;

  NSRect frame = NSMakeRect(x, y, width, height);
  
  NSUInteger windowStyle = NSWindowStyleMaskTitled | NSWindowStyleMaskMiniaturizable;

  NSWindow* window = [[NSWindow alloc] initWithContentRect:frame
                       		             styleMask:windowStyle
                                   	   backing:NSBackingStoreBuffered
                                  	   defer:NO];
window.titlebarAppearsTransparent = true;
  NSProgressIndicator *progressIndicator = [[NSProgressIndicator alloc] initWithFrame:NSMakeRect(32, 0, width - 64, height)];
  [progressIndicator setStyle:NSProgressIndicatorBarStyle];
  [progressIndicator setControlSize:NSControlSizeRegular];

  [progressIndicator setMaxValue:100];
	[progressIndicator setDoubleValue:0];

  [[window contentView] addSubview:progressIndicator];
  app->progressIndicator = progressIndicator;
  app->win = window;

  [progressIndicator startAnimation:nil];

  [window makeKeyAndOrderFront:nil];
  [window orderFront:nil];
  [window orderFrontRegardless];

  NSButton *closeButton = [[NSButton alloc] initWithFrame:NSMakeRect(width - 24, 15, 16, 16)];
  [closeButton setButtonType:NSMomentaryLightButton];
  [closeButton setBezelStyle:NSRoundedBezelStyle]; 

  NSImage *crossImage = [NSImage imageNamed:NSImageNameStopProgressFreestandingTemplate];
  [closeButton setImage:crossImage];

  [[window contentView] addSubview:closeButton];

  NSTextField *textField = [[NSTextField alloc] initWithFrame:NSMakeRect(30, 24, 200, 22)];
  [textField setStringValue:@"Calculating…"];
  [[window contentView] addSubview:textField];

  [textField setFont:[NSFont systemFontOfSize:12]];

  textField.bezeled = NO;
  textField.editable = NO;
  textField.drawsBackground = NO;

  NSTextField *timer = [[NSTextField alloc] initWithFrame:NSMakeRect(30, -4, 200, 22)];

  [[window contentView] addSubview:timer];

  [timer setFont:[NSFont systemFontOfSize:11]];

  timer.bezeled = NO;
  timer.editable = NO;
  timer.drawsBackground = NO;

  timer.textColor = [NSColor grayColor];
  app->timer = timer;

  return window;
}

void progressIndicator(t_app app, double value) {
  dispatch_async(dispatch_get_main_queue(), ^{
    [(id)app.progressIndicator setIndeterminate:NO];
    [(id)app.progressIndicator setDoubleValue:value];
  });
}

void timer(t_app app, char *str) {

  NSString* string = [NSString stringWithCString:str encoding:NSASCIIStringEncoding];

  dispatch_async(dispatch_get_main_queue(), ^{
    [(id)app.timer setStringValue:string];
  });
}
