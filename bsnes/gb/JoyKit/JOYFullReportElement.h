#import <Foundation/Foundation.h>
#import <IOKit/hid/IOHIDLib.h>
#import "JOYElement.h"

@interface JOYFullReportElement : JOYElement<NSCopying>
- (instancetype)initWithDevice:(IOHIDDeviceRef) device reportID:(unsigned)reportID;
- (void)updateValue:(NSData *)value;
@end


