#import "GBSplitView.h"

@implementation GBSplitView
{
    NSColor *_dividerColor;
}

- (void)setDividerColor:(NSColor *)color 
{
    _dividerColor = color;
    [self setNeedsDisplay:true];
}

- (NSColor *)dividerColor 
{
    if (_dividerColor) {
        return _dividerColor;
    }
    return [super dividerColor];
}

- (void)drawDividerInRect:(NSRect)rect
{
    [self.dividerColor set];
    NSRectFill(rect);
}

/* Mavericks comaptibility */
- (NSArray<NSView *> *)arrangedSubviews
{
    if (@available(macOS 10.11, *)) {
        return [super arrangedSubviews];
    }
    else {
        return [self subviews];
    }
}

@end
