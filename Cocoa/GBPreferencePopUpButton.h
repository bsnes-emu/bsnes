#import <Cocoa/Cocoa.h>

@interface GBPreferenceMenuItem : NSMenuItem
@property (nonatomic) IBInspectable NSString *preferenceValue;
@end

@interface GBPreferencePopUpButton : NSPopUpButton
@property (nonatomic) IBInspectable NSString *preferenceName;
@end
