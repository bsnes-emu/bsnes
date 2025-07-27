#import "NSTextFieldCell+Inset.h"
#import <AppKit/AppKit.h>
#import <objc/runtime.h>

@interface NSTextFieldCell ()
- (CGRect)_textLayerDrawingRectForCellFrame:(CGRect)rect;
@property NSSize textInset;
@end

@implementation NSTextFieldCell (Inset)

- (void)setTextInset:(NSSize)textInset
{
    objc_setAssociatedObject(self, @selector(textInset), @(textInset), OBJC_ASSOCIATION_RETAIN);
}

- (NSSize)textInset
{
    return [objc_getAssociatedObject(self, _cmd) sizeValue];
}

- (CGRect)_textLayerDrawingRectForCellFrameHook:(CGRect)rect
{
    CGRect ret = [self _textLayerDrawingRectForCellFrameHook:rect];
    NSSize inset = self.textInset;
    ret.origin.x += inset.width;
    ret.origin.y += inset.height;
    ret.size.width -= inset.width;
    ret.size.height -= inset.height;
    return ret;
}

+ (void)load
{
    method_exchangeImplementations(class_getInstanceMethod(self, @selector(_textLayerDrawingRectForCellFrame:)),
                                   class_getInstanceMethod(self, @selector(_textLayerDrawingRectForCellFrameHook:)));
}

@end
