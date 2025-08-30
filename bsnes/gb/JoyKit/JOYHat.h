#import <Foundation/Foundation.h>
#import "JOYInput.h"

@interface JOYHat : JOYInput
- (double)angle;
- (unsigned)resolution;
@property (readonly, getter=isPressed) bool pressed;

@end


