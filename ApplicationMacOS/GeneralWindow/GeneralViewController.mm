#include "GeneralViewController.hpp"

@implementation GeneralViewController

- (void) viewDidLoad
{
	[super viewDidLoad];

	[self setupRenderButton];
}

- (void) setupRenderButton
{
	auto button = [NSButton new];

    [button setTarget:self];
    [button setAction:@selector(didTapOnRenderButton:)];
    [button setTitle:@"Render"];
	 
    [button setTranslatesAutoresizingMaskIntoConstraints:NO];

	auto constraints = @[
        [NSLayoutConstraint constraintWithItem:button attribute:NSLayoutAttributeLeading relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeLeading multiplier:1.0 constant:12],
        [NSLayoutConstraint constraintWithItem:button attribute:NSLayoutAttributeTrailing relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeTrailing multiplier:1.0 constant:-12],
        [NSLayoutConstraint constraintWithItem:button attribute:NSLayoutAttributeTop relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeTop multiplier:1.0 constant:0],
        [NSLayoutConstraint constraintWithItem:button attribute:NSLayoutAttributeBottom relatedBy:NSLayoutRelationEqual toItem:self.view attribute:NSLayoutAttributeBottom multiplier:1.0 constant:-8]
    ];

    [self.view addSubview:button];
	[self.view addConstraints:constraints];
}

- (void) didTapOnRenderButton:(id)sender
{
	if (self.model.render.has_value()) {
		(*self.model.render)();
	}
}

@end
