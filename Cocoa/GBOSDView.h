#import <Cocoa/Cocoa.h>

@interface GBOSDView : NSView
@property bool usesSGBScale;
- (void)displayText:(NSString *)text;
@end
