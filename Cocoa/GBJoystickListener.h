#import <AppKit/AppKit.h>

@protocol GBJoystickListener <NSObject>

- (void) joystick:(NSString *)joystick_name button: (unsigned)button changedState: (bool) state;
- (void) joystick:(NSString *)joystick_name axis: (unsigned)axis movedTo: (signed) value;

@end
