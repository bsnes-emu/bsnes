#import <Cocoa/Cocoa.h>
#import "GBShader.h"
#include "gb.h"

@interface GBView : NSOpenGLView
- (void) flip;
- (uint32_t *) pixels;
@property GB_gameboy_t *gb;
@property (nonatomic) BOOL shouldBlendFrameWithPrevious;
@property GBShader *shader;
@property (getter=isMouseHidingEnabled) BOOL mouseHidingEnabled;
@end
