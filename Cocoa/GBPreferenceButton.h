#import <Cocoa/Cocoa.h>

@interface GBPreferenceButton : NSButton
@property (nonatomic) IBInspectable NSString *preferenceName;
@property IBInspectable BOOL invertValue;
@end
