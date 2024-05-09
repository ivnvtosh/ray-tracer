#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#import <Cocoa/Cocoa.h>

#include "Model.hpp"

@interface MainWindow : NSWindow

@property Model model;

@end

#endif  // MAIN_WINDOW_H
