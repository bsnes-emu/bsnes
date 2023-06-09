#import <TargetConditionals.h>
#import <MetalKit/MetalKit.h>
#if TARGET_OS_IPHONE
#import "../iOS/GBView.h"
#else
#import "../Cocoa/GBView.h"
#endif

@interface GBViewMetal : GBView<MTKViewDelegate>
+ (bool) isSupported;
@end
