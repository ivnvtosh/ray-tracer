#ifndef RENDER_VIEW_CONTROLLER_HPP
#define RENDER_VIEW_CONTROLLER_HPP

#import <Cocoa/Cocoa.h>

@interface RenderViewController : NSViewController

@property (nonatomic, strong) NSOpenGLView *openGLView;

- (void) selectOpenGLContext;

@end

#endif  // RENDER_VIEW_CONTROLLER_HPP
