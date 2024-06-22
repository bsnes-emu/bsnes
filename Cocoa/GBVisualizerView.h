#import <Cocoa/Cocoa.h>
#import <Core/gb.h>

@interface GBVisualizerView : NSView
- (void)addSample:(GB_sample_t *)sample;
@end
