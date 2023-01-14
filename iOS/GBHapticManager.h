#import <Foundation/Foundation.h>

@interface GBHapticManager : NSObject
+ (instancetype)sharedManager;
- (void)doTapHaptic;
- (void)setRumbleStrength:(double)rumble;
@end
