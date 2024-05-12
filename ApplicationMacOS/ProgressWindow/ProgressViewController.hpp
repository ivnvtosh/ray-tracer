#ifndef PROGRESS_VIEW_CONTROLLER_HPP
#define PROGRESS_VIEW_CONTROLLER_HPP

#import <Cocoa/Cocoa.h>

#include <chrono>

#include "ProgressWindowModel.hpp"

using TimeDuration = std::chrono::duration<double, std::milli>;;

@interface ProgressViewController : NSViewController

@property ProgressWindowModel model;

@property (nonatomic, strong) NSStackView *horizontalStackView;
@property (nonatomic, strong) NSStackView *verticalStackView;
@property (nonatomic, strong) NSButton *pauseButton;
@property (nonatomic, strong) NSButton *playButton;

@property (nonatomic, strong) NSProgressIndicator *progressIndicator;

@property (nonatomic, strong) NSTextField *titleTextField;
@property (nonatomic, strong) NSTextField *timerTextField;
@property (nonatomic, strong) NSTimer *timer;
@property TimeDuration duration;

- (void) update:(TimeDuration)duration with:(double)value;
- (void) done:(TimeDuration)duration;

@end

#endif  // PROGRESS_VIEW_CONTROLLER_HPP
