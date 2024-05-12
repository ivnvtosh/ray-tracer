#define GL_SILENCE_DEPRECATION
#import <OpenGL/gl3.h>

#include "RenderViewController.hpp"

@implementation RenderViewController

- (void) viewDidLoad
{
	[super viewDidLoad];

	[self setupOpenGLView]; 
}

- (void) setupOpenGLView
{
  NSRect frame = NSMakeRect(0, 0, 512, 512);
  NSOpenGLPixelFormatAttribute pfa_attrs[] = {NSOpenGLPFADepthSize, 32, NSOpenGLPFAOpenGLProfile,  NSOpenGLProfileVersionLegacy, 0};
  auto pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:pfa_attrs];

	auto openGLView = [[NSOpenGLView alloc] initWithFrame:frame pixelFormat:pixFmt];

  [[openGLView openGLContext] makeCurrentContext];
  [[openGLView openGLContext] setView:openGLView];
  
  [openGLView prepareOpenGL];
  [openGLView setNextKeyView:openGLView];
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();

  [openGLView setTranslatesAutoresizingMaskIntoConstraints:NO];

	auto constraints = @[
        [NSLayoutConstraint constraintWithItem:openGLView attribute:NSLayoutAttributeLeading relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeLeading multiplier:1.0 constant:0],
        [NSLayoutConstraint constraintWithItem:openGLView attribute:NSLayoutAttributeTrailing relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeTrailing multiplier:1.0 constant:0],
        [NSLayoutConstraint constraintWithItem:openGLView attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeTop multiplier:1.0 constant:0],
        [NSLayoutConstraint constraintWithItem:openGLView attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeBottom multiplier:1.0 constant:0]
  ];

  [self.view addSubview:openGLView];
	[self.view addConstraints:constraints];

  self.openGLView = openGLView;

  [pixFmt release];
}

- (void) selectOpenGLContext
{
  if ([self.openGLView openGLContext] != [NSOpenGLContext currentContext])
  {
    [[self.openGLView openGLContext] makeCurrentContext];
  }
}

@end
