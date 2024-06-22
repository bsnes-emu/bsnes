#import <AppKit/AppKit.h>

@interface NSObject (DefaultsObserver)
- (void)observeStandardDefaultsKey:(NSString *)key withBlock:(void(^)(id newValue))block;
- (void)observeStandardDefaultsKey:(NSString *)key selector:(SEL)selector;
@end
