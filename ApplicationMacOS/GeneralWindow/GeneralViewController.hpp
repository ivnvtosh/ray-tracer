#ifndef GENERAL_VIEW_CONTROLLER_HPP
#define GENERAL_VIEW_CONTROLLER_HPP

#import <Cocoa/Cocoa.h>

#include "GeneralWindowModel.hpp"

@interface GeneralViewController : NSViewController

@property GeneralWindowModel model;

@end

#endif  // GENERAL_VIEW_CONTROLLER_HPP
