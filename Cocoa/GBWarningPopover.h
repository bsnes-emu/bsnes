#import <Cocoa/Cocoa.h>

@interface GBWarningPopover : NSPopover

+ (GBWarningPopover *) popoverWithContents:(NSString *)contents onView:(NSView *)view;
+ (GBWarningPopover *) popoverWithContents:(NSString *)contents onWindow:(NSWindow *)window;
+ (GBWarningPopover *) popoverWithContents:(NSString *)contents title:(NSString *)title onView:(NSView *)view timeout:(double)seconds preferredEdge:(NSRectEdge)preferredEdge;

@end
