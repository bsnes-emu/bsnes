//
//  GBSplitView.m
//  SameBoySDL
//
//  Created by Lior Halphon on 9/4/20.
//  Copyright © 2020 Lior Halphon. All rights reserved.
//

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
