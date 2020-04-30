#import <Foundation/Foundation.h>
#include <IOKit/hid/IOHIDLib.h>

@interface JOYElement : NSObject<NSCopying>
- (instancetype)initWithElement:(IOHIDElementRef)element;
- (int32_t)value;
- (NSData *)dataValue;
- (void)setValue:(uint32_t)value;
- (void)setDataValue:(NSData *)value;
@property (readonly) uint16_t usage;
@property (readonly) uint16_t usagePage;
@property (readonly) uint32_t uniqueID;
@property unsigned persistentUniqueID;
@property int32_t min;
@property int32_t max;
@property (readonly) int32_t reportID;
@property (readonly) int32_t parentID;

@end


