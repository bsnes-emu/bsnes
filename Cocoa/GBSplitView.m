#import "GBSplitView.h"

@implementation GBSplitView
{
    NSColor *_dividerColor;
}

- (void)setDividerColor:(NSColor *)color {
    _dividerColor = color;
    [self setNeedsDisplay:YES];
}

- (NSColor *)dividerColor {
    if (_dividerColor) {
        return _dividerColor;
    }
    return [super dividerColor];
}

@end
