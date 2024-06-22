#import <Foundation/Foundation.h>
#import <IOKit/hid/IOHIDLib.h>

#define inline_const(type, ...) (*({static const typeof(type) _= __VA_ARGS__; &_;}))

@interface JOYElement : NSObject<NSCopying>
- (instancetype)initWithElement:(IOHIDElementRef)element;
- (int32_t)value;
- (NSData *)dataValue;
- (IOReturn)setValue:(uint32_t)value;
- (IOReturn)setDataValue:(NSData *)value;
@property (readonly) uint16_t usage;
@property (readonly) uint16_t usagePage;
@property (readonly) uint32_t uniqueID;
@property int32_t min;
@property int32_t max;
@property (readonly) int32_t reportID;
@property (readonly) int32_t parentID;

@end


