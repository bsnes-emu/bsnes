#import <Cocoa/Cocoa.h>
#include <Core/gb.h>
#import "GBShader.h"

@interface GBView : NSOpenGLView
- (void) flip;
- (uint32_t *) pixels;
@property GB_gameboy_t *gb;
@property (nonatomic) BOOL shouldBlendFrameWithPrevious;
@property GBShader *shader;
@property (getter=isMouseHidingEnabled) BOOL mouseHidingEnabled;
@end
