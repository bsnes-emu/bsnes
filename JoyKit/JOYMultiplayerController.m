#import "JOYMultiplayerController.h"

@interface JOYController ()
- (instancetype)initWithDevice:(IOHIDDeviceRef)device reportIDFilter:(NSArray <NSNumber *> *) filter serialSuffix:(NSString *)suffix;
- (void)elementChanged:(IOHIDElementRef) element toValue:(IOHIDValueRef) value;
- (void)disconnected;
@end

@implementation JOYMultiplayerController
{
    NSMutableArray <JOYController *> *_children;
}

- (instancetype)initWithDevice:(IOHIDDeviceRef) device reportIDFilters:(NSArray <NSArray <NSNumber *> *>*) reportIDFilters
{
    self = [super init];
    if (!self) return self;
    
    _children = [NSMutableArray array];
    
    unsigned index = 1;
    for (NSArray *filter in reportIDFilters) {
        JOYController *controller = [[JOYController alloc] initWithDevice:device reportIDFilter:filter serialSuffix:[NSString stringWithFormat:@"%d", index]];
        [_children addObject:controller];
        index++;
    }
    return self;
}

- (void)elementChanged:(IOHIDElementRef) element toValue:(IOHIDValueRef) value
{
    for (JOYController *child in _children) {
        [child elementChanged:element toValue:value];
    }
}

- (void)disconnected
{
    for (JOYController *child in _children) {
        [child disconnected];
    }
}

@end
