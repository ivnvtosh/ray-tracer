#include <string>
#include <sstream>

#include "ProgressViewController.hpp"

@implementation ProgressViewController

- (void) viewDidLoad
{
	[super viewDidLoad];

	[self setupHorizontalStackView];
	[self setupVerticalStackView];
	[self setupTitleTextField];
	[self setupProgressIndicator];
	[self setupTimerTextField];
	[self setupPauseButton];
	[self setupPlayButton];
	[self setupCancelButton];
}

- (void) setupHorizontalStackView
{
	auto stackView = [NSStackView new];

    [stackView setOrientation: NSUserInterfaceLayoutOrientationHorizontal];
    [stackView setAlignment: NSLayoutAttributeCenterY];
    [stackView setSpacing:12];
    [stackView setTranslatesAutoresizingMaskIntoConstraints:NO];
	[stackView setDistribution:NSStackViewDistributionFill];

	auto constraints = @[
        [NSLayoutConstraint constraintWithItem:stackView attribute:NSLayoutAttributeLeading relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeLeading multiplier:1.0 constant:12],
        [NSLayoutConstraint constraintWithItem:stackView attribute:NSLayoutAttributeTrailing relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeTrailing multiplier:1.0 constant:-12],
        [NSLayoutConstraint constraintWithItem:stackView attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeTop multiplier:1.0 constant:0],
        [NSLayoutConstraint constraintWithItem:stackView attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeBottom multiplier:1.0 constant:-8]
    ];

    [self.view addSubview:stackView];
	[self.view addConstraints:constraints];

	self.horizontalStackView = stackView;
}

- (void) setupVerticalStackView
{
	auto stackView = [NSStackView new];

    [stackView setOrientation: NSUserInterfaceLayoutOrientationHorizontal];
    [stackView setAlignment: NSLayoutAttributeLeading];
    [stackView setSpacing:0];

	[self.horizontalStackView addView:stackView inGravity:NSStackViewGravityCenter];

	self.verticalStackView = stackView;
}

- (void) setupTitleTextField
{
	auto textField = [NSTextField new];

	[textField setBezeled:NO];
	[textField setDrawsBackground:NO];
	[textField setEditable:NO];
	[textField setStringValue:@"Rendering…"];
	[textField setFont:[NSFont systemFontOfSize:12]];

	[self.verticalStackView addView:textField inGravity:NSStackViewGravityCenter];

	self.titleTextField = textField;
}

- (void) setupProgressIndicator
{
	auto progressIndicator = [NSProgressIndicator new];
	
	[progressIndicator setStyle:NSProgressIndicatorBarStyle];
	[progressIndicator setControlSize:NSControlSizeRegular];
	[progressIndicator setMaxValue:100];
	[progressIndicator setDoubleValue:0];
	[progressIndicator startAnimation:nil];

	[self.verticalStackView addView:progressIndicator inGravity:NSStackViewGravityCenter];

	self.progressIndicator = progressIndicator;
}

- (void) setupTimerTextField
{
	auto textField = [NSTextField new];

	[textField setBezeled:NO];
	[textField setDrawsBackground:NO];
	[textField setEditable:NO];
	[textField setStringValue:@"Calculating"];
	[textField setFont:[NSFont systemFontOfSize:12]];
    [textField setTextColor:[NSColor grayColor]];

	[self.verticalStackView addView:textField inGravity:NSStackViewGravityCenter];

	self.timerTextField = textField;
}

- (void) startTimer
{
	self.timer = [NSTimer scheduledTimerWithTimeInterval:1.0
						  target:self
						  selector:@selector(timerFired:)
						  userInfo:nil
						  repeats:YES];
}

- (void) stopTimer
{
	[self.timer invalidate];
	[self setTimer:nil];
}

- (void) timerFired:(NSTimer *)timer
{
	using namespace std::chrono;

	self.duration -= std::chrono::seconds(1);

	if (duration_cast<seconds>(self.duration) <= seconds(0)) {
		dispatch_async(dispatch_get_main_queue(), ^{
			[self.timerTextField setStringValue:@"One moment"];
		});

		[self progressDone:seconds(12421412)];
		return;
	}

	auto text = [self durationToString:self.duration];

	dispatch_async(dispatch_get_main_queue(), ^{
		[self.timerTextField setStringValue:text];
	});
}

- (NSString*) durationToString:(TimeDuration) duration
{
	using namespace std::chrono;

	auto d = duration_cast<hours>(duration) / 24;
	auto h = duration_cast<hours>(duration - d * 24);
  	auto m = duration_cast<minutes>(duration - d * 24 - h);
  	auto s = duration_cast<seconds>(duration - d * 24 - h - m);

	std::stringstream stream;

	if (auto days = d.count()) {
		stream << days << " days ";
	}

	if (auto hours = h.count()) {
		stream << hours << " hours ";
	}

	if (auto minutes = m.count()) {
		stream << minutes << " minutes ";
	}

	if (auto seconds = s.count()) {
		stream << seconds << " seconds ";
	}

	auto stringtext = stream.str();
	auto ctext = stringtext.c_str();
	auto text = [NSString stringWithUTF8String:ctext];

	return text;
}

// Pause Button

- (void) setupPauseButton
{
	auto button = [NSButton new];

    [button setTarget:self];
    [button setAction:@selector(didTapOnPauseButton:)];

	auto image = [NSImage imageWithSystemSymbolName:@"pause.circle.fill"];

	[button setImage:image];
	[button setBordered: NO];

	[self.horizontalStackView addView:button inGravity:NSStackViewGravityCenter];

	self.pauseButton = button;
}

- (void) didTapOnPauseButton:(id)sender
{
	[self stopTimer];

	dispatch_async(dispatch_get_main_queue(), ^{
		[self.pauseButton setHidden:YES];
		[self.playButton setHidden:NO];
		[self.progressIndicator stopAnimation:nil];
	});

	if (self.model.pause.has_value()) {
		(*self.model.pause)();
	}
}

// Play Button

- (void) setupPlayButton
{
	auto button = [NSButton new];

    [button setTarget:self];
    [button setAction:@selector(didTapOnPlayButton:)];

	auto image = [NSImage imageWithSystemSymbolName:@"play.circle.fill"];

	[button setImage:image];
	[button setBordered:NO];
	[button setHidden:YES];
	
	[self.horizontalStackView addView:button inGravity:NSStackViewGravityCenter];

	self.playButton = button;
}

- (void) didTapOnPlayButton:(id)sender
{
	[self startTimer];

	dispatch_async(dispatch_get_main_queue(), ^{
		[self.pauseButton setHidden:NO];
		[self.playButton setHidden:YES];
		[self.progressIndicator startAnimation:nil];
	});

	if (self.model.pause.has_value()) {
		(*self.model.pause)();
	}	
}

// Cancel Button

- (void) setupCancelButton
{
	auto button = [NSButton new];

    [button setTarget:self];
    [button setAction:@selector(didTapOnCancelButton:)];

	auto image = [NSImage imageWithSystemSymbolName:@"xmark.circle.fill"];

	[button setImage:image];
	[button setBordered: NO];

	[self.horizontalStackView addView:button inGravity:NSStackViewGravityCenter];
}

- (void) didTapOnCancelButton:(id)sender
{
	[self.view.window close];

	if (self.model.cancel.has_value()) {
		(*self.model.cancel)();
	}
}

- (void) update:(TimeDuration)duration with:(double)value
{
	[self stopTimer];
	[self setDuration:duration];
	[self startTimer];

	dispatch_async(dispatch_get_main_queue(), ^{
		[self.progressIndicator setIndeterminate:NO];
		[self.progressIndicator setDoubleValue:value];
	});
}

- (void) done:(TimeDuration)duration
{
	dispatch_async(dispatch_get_main_queue(), ^{
		[self.titleTextField setStringValue:@"Rendering is complete."];

		auto text = [NSString stringWithFormat:@"Total %@", [self durationToString:duration]];

		[self.timerTextField setStringValue:text];

		[self.playButton setHidden:YES];

		[self.pauseButton setHidden:NO];
		[self.pauseButton setEnabled:NO];

		[self.progressIndicator setDoubleValue:100];
		[self.progressIndicator setIndeterminate:NO];
		[self.progressIndicator stopAnimation:nil];
	});
}

@end
