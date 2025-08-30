#import <Cocoa/Cocoa.h>

@interface GBPreferencesSlider : NSSlider
@property (nonatomic) IBInspectable NSString *preferenceName;
@property IBInspectable unsigned denominator;
@end
