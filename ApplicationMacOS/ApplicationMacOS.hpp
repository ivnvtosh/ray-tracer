#ifndef APPLICATION_MACOS_HPP
#define APPLICATION_MACOS_HPP

#define GL_SILENCE_DEPRECATION

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl3.h>

#include "MainWindow.hpp"

@interface OpenGLView : NSOpenGLView {
  int width;
  int height;
}

- (void)selectGLContext;

@end

#endif  // APPLICATION_MACOS_HPP
