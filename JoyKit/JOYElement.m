#import "JOYElement.h"
#include <IOKit/hid/IOHIDLib.h>

@implementation JOYElement
{
    id _element;
    IOHIDDeviceRef _device;
    int32_t _min, _max;
}

- (int32_t)min
{
    return MIN(_min, _max);
}

- (int32_t)max
{
    return MAX(_max, _min);
}

-(void)setMin:(int32_t)min
{
    _min = min;
}

- (void)setMax:(int32_t)max
{
    _max = max;
}

- (instancetype)initWithElement:(IOHIDElementRef)element
{
    if ((self = [super init])) {
        _element = (__bridge id)element;
        _usage = IOHIDElementGetUsage(element);
        _usagePage = IOHIDElementGetUsagePage(element);
        _uniqueID = (uint32_t)IOHIDElementGetCookie(element);
        _min = (int32_t) IOHIDElementGetLogicalMin(element);
        _max = (int32_t) IOHIDElementGetLogicalMax(element);
        _reportID = IOHIDElementGetReportID(element);
        IOHIDElementRef parent = IOHIDElementGetParent(element);
        _parentID = parent? (uint32_t)IOHIDElementGetCookie(parent) : -1;
        _device = IOHIDElementGetDevice(element);
        
        /* Catalina added a new input type in a way that breaks cookie consistency across macOS versions,
           we shall adjust our cookies to to compensate */
        unsigned cookieShift = 0, parentCookieShift = 0;
        NSArray *nones = CFBridgingRelease(IOHIDDeviceCopyMatchingElements(IOHIDElementGetDevice(element),
                                                                           (__bridge CFDictionaryRef)@{@(kIOHIDElementTypeKey): @(kIOHIDElementTypeInput_NULL)},
                                                                           0));
        for (id none in nones) {
            if (IOHIDElementGetCookie((__bridge IOHIDElementRef) none) < _uniqueID) {
                cookieShift++;
            }
            if (IOHIDElementGetCookie((__bridge IOHIDElementRef) none) < (int32_t)_parentID) {
                parentCookieShift++;
            }
        }
        
        _uniqueID -= cookieShift;
        _parentID -= parentCookieShift;
    }
    return self;
}

- (int32_t)value
{
    IOHIDValueRef value = NULL;
    IOHIDDeviceGetValue(_device, (__bridge IOHIDElementRef)_element, &value);
    if (!value) return 0;
    CFRelease(CFRetain(value)); // For some reason, this is required to prevent leaks.
    return (int32_t)IOHIDValueGetIntegerValue(value);
}

- (NSData *)dataValue
{
    IOHIDValueRef value = NULL;
    IOHIDDeviceGetValue(_device, (__bridge IOHIDElementRef)_element, &value);
    if (!value) return 0;
    CFRelease(CFRetain(value)); // For some reason, this is required to prevent leaks.
    return [NSData dataWithBytes:IOHIDValueGetBytePtr(value) length:IOHIDValueGetLength(value)];
}

- (void)setValue:(uint32_t)value
{
    IOHIDValueRef ivalue = IOHIDValueCreateWithIntegerValue(NULL, (__bridge IOHIDElementRef)_element, 0, value);
    IOHIDDeviceSetValue(_device, (__bridge IOHIDElementRef)_element, ivalue);
    CFRelease(ivalue);
}

- (void)setDataValue:(NSData *)value
{
    IOHIDValueRef ivalue = IOHIDValueCreateWithBytes(NULL, (__bridge IOHIDElementRef)_element, 0, value.bytes, value.length);
    IOHIDDeviceSetValue(_device, (__bridge IOHIDElementRef)_element, ivalue);
    CFRelease(ivalue);
}

/* For use as a dictionary key */

- (NSUInteger)hash
{
    return self.uniqueID;
}

- (BOOL)isEqual:(id)object
{
    return self->_element == object;
}

- (id)copyWithZone:(nullable NSZone *)zone;
{
    return self;
}
@end
