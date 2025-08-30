#import "NSObject+DefaultsObserver.h"
#import <AppKit/AppKit.h>
#import <objc/runtime.h>

@interface GBUserDefaultsObserverHelper : NSObject
@end

@implementation GBUserDefaultsObserverHelper
+ (void)observeValueForKeyPath:(NSString *)keyPath
                      ofObject:(id)object
                        change:(NSDictionary<NSKeyValueChangeKey,id> *)change
                       context:(void *)context
{
    [[NSNotificationCenter defaultCenter] postNotificationName:[keyPath stringByAppendingString:@"Changed$DefaultsObserver"]
                                                        object:nil
                                                      userInfo:@{
                                                          @"value": change[NSKeyValueChangeNewKey]
                                                      }];
}

+ (void)startObservingKey:(NSString *)key
{
    if (!NSThread.isMainThread) {
        dispatch_sync(dispatch_get_main_queue(), ^{
            [self startObservingKey:key];
        });
        return;
    }
    static NSMutableSet *set = nil;
    if (!set) {
        set = [NSMutableSet set];
    }
    if ([set containsObject:key]) return;
    [set addObject:key];
    [[NSUserDefaults standardUserDefaults] addObserver:(id)self
                                            forKeyPath:key
                                               options:NSKeyValueObservingOptionNew
                                               context:nil];
}
@end

@implementation NSObject (DefaultsObserver)
- (void)observeStandardDefaultsKey:(NSString *)key selector:(SEL)selector
{
    __weak id weakSelf = self;
    [self observeStandardDefaultsKey:key
                           withBlock:^(id newValue) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Warc-performSelector-leaks"
        [weakSelf performSelector:selector withObject:newValue];
#pragma clang diagnostic pop
    }];
}

- (void)observeStandardDefaultsKey:(NSString *)key withBlock:(void(^)(id newValue))block
{
    NSString *notificationName = [key stringByAppendingString:@"Changed$DefaultsObserver"];
    objc_setAssociatedObject(self, sel_registerName(notificationName.UTF8String), block, OBJC_ASSOCIATION_RETAIN);
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(standardDefaultsKeyChanged:)
                                                 name:notificationName
                                               object:nil];
    [GBUserDefaultsObserverHelper startObservingKey:key];
    block([[NSUserDefaults standardUserDefaults] objectForKey:key]);
}

- (void)standardDefaultsKeyChanged:(NSNotification *)notification
{
    SEL selector = sel_registerName(notification.name.UTF8String);
    ((void(^)(id))objc_getAssociatedObject(self, selector))(notification.userInfo[@"value"]);
}
@end
