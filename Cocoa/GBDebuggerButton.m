#import "GBDebuggerButton.h"

@implementation GBDebuggerButton
{
    NSTrackingArea *_trackingArea;
}
- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    self.toolTip = self.title;
    return self;
}

- (void)mouseEntered:(NSEvent *)event
{
    if (@available(macOS 10.10, *)) {
        NSDictionary *attributes = @{
            NSForegroundColorAttributeName: [NSColor colorWithWhite:1.0 alpha:0.5],
            NSFontAttributeName: self.textField.font
        };
        self.textField.placeholderAttributedString =
            [[NSAttributedString alloc] initWithString:self.alternateTitle attributes:attributes];
    }
}

- (void)mouseExited:(NSEvent *)event
{
    if (@available(macOS 10.10, *)) {
        if ([self.textField.placeholderAttributedString.string isEqualToString:self.alternateTitle]) {
            self.textField.placeholderAttributedString = nil;
        }
    }
}

-(void)updateTrackingAreas
{
    [super updateTrackingAreas];
    if (_trackingArea) {
        [self removeTrackingArea:_trackingArea];
    }
    
    _trackingArea = [ [NSTrackingArea alloc] initWithRect:[self bounds]
                                                  options:NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways
                                                    owner:self
                                                 userInfo:nil];
    [self addTrackingArea:_trackingArea];
}
@end
