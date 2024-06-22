#import "GBTintedImageCell.h"

@implementation GBTintedImageCell

- (NSImage *)image
{
    if (!self.tint || !super.image.isTemplate) {
        return [super image];
    }
    
    NSImage *tinted = [super.image copy];
    [tinted lockFocus];
    [self.tint set];
    NSRectFillUsingOperation((NSRect){.size = tinted.size}, NSCompositeSourceIn);
    [tinted unlockFocus];
    tinted.template = false;
    return tinted;
}

@end
