#import <Cocoa/Cocoa.h>
#import <OpenGL/gl3.h>

#define GL_SILENCE_DEPRECATION

@interface MyOpenGLView : NSOpenGLView

@end

@implementation MyOpenGLView

- (instancetype)initWithFrame:(NSRect)frameRect {
  NSOpenGLPixelFormatAttribute attributes[] = {
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFADepthSize, 
	24,
    0
  };
  NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attributes];
  self = [super initWithFrame:frameRect pixelFormat:pixelFormat];
  return self;
}

- (void)drawRect:(NSRect)dirtyRect {
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  [[self openGLContext] flushBuffer];
}

@end

void render() {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

  [NSApplication sharedApplication];

  NSRect frame = NSMakeRect(0, 0, 512, 512);
  NSWindow *window = [[NSWindow alloc] initWithContentRect:frame
                                       styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable
                                       backing:NSBackingStoreBuffered
                                       defer:NO];
  [window setTitle:@"Ray Tracer"];

  MyOpenGLView *glView = [[MyOpenGLView alloc] initWithFrame:[window.contentView bounds]];

  [window.contentView addSubview:glView];
  [window makeKeyAndOrderFront:nil];

  [NSApp run];

  [pool drain];
}
