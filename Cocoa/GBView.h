#import <Cocoa/Cocoa.h>
#include "gb.h"

@interface GBView : NSOpenGLView
- (void) flip;
- (uint32_t *) pixels;
@property GB_gameboy_t *gb;
@property BOOL shouldBlendFrameWithPrevious;
@end
