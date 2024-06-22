#import "GBWarningPopover.h"

static GBWarningPopover *lastPopover;

@implementation GBWarningPopover

+ (GBWarningPopover *) popoverWithContents:(NSString *)contents title:(NSString *)title onView:(NSView *)view timeout:(double)seconds preferredEdge:(NSRectEdge)preferredEdge
{
    [lastPopover close];
    lastPopover = [[self alloc] init];
    
    [lastPopover setBehavior:NSPopoverBehaviorApplicationDefined];
    [lastPopover setAnimates:true];
    lastPopover.contentViewController = [[NSViewController alloc] initWithNibName:@"PopoverView" bundle:nil];
    NSTextField *field = (NSTextField *)lastPopover.contentViewController.view;
    if (!title) {
        [field setStringValue:contents];
    }
    else {
        NSMutableAttributedString *fullContents = [[NSMutableAttributedString alloc] initWithString:title
                                                                                         attributes:@{NSFontAttributeName: [NSFont boldSystemFontOfSize:[NSFont systemFontSize]]}];
        [fullContents appendAttributedString:[[NSAttributedString alloc] initWithString:[@"\n" stringByAppendingString:contents]
                                                                             attributes:@{NSFontAttributeName: [NSFont systemFontOfSize:[NSFont systemFontSize]]}]];
        [field setAttributedStringValue:fullContents];
        
    }
    NSSize textSize = [field.cell cellSizeForBounds:[field.cell drawingRectForBounds:NSMakeRect(0, 0, 240, CGFLOAT_MAX)]];
    textSize.width = ceil(textSize.width) + 16;
    textSize.height = ceil(textSize.height) + 12;
    [lastPopover setContentSize:textSize];
    
    if (!view.window.isVisible) {
        [view.window setIsVisible:true];
    }
    
    [lastPopover showRelativeToRect:view.bounds
                             ofView:view
                      preferredEdge:preferredEdge];
    
    NSRect frame = field.frame;
    frame.origin.x += 8;
    frame.origin.y += 6;
    frame.size.width -= 16;
    frame.size.height -= 12;
    field.frame = frame;
    

    [lastPopover performSelector:@selector(close) withObject:nil afterDelay:3.0];
    
    return lastPopover;
}

+ (GBWarningPopover *) popoverWithContents:(NSString *)contents onView:(NSView *)view
{
    return [self popoverWithContents:contents title:nil onView:view timeout:3.0 preferredEdge:NSMinYEdge];
}

+ (GBWarningPopover *)popoverWithContents:(NSString *)contents onWindow:(NSWindow *)window
{
    return [self popoverWithContents:contents onView:window.contentView.superview.subviews.lastObject];
}

@end
