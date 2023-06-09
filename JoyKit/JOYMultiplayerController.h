#import "JOYController.h"
#import <IOKit/hid/IOHIDLib.h>

@interface JOYMultiplayerController : JOYController
- (instancetype)initWithDevice:(IOHIDDeviceRef) device reportIDFilters:(NSArray <NSArray <NSNumber *> *>*) reportIDFilters hacks:hacks;
@end


