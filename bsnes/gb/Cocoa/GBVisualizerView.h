#import <Cocoa/Cocoa.h>
#include <Core/gb.h>

@interface GBVisualizerView : NSView
- (void)addSample:(GB_sample_t *)sample;
@end
