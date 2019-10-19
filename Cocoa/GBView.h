#import <Cocoa/Cocoa.h>
#include <Core/gb.h>
#import <JoyKit/JoyKit.h>

@interface GBView : NSView<JOYListener>
- (void) flip;
- (uint32_t *) pixels;
@property GB_gameboy_t *gb;
@property (nonatomic) BOOL shouldBlendFrameWithPrevious;
@property (getter=isMouseHidingEnabled) BOOL mouseHidingEnabled;
@property bool isRewinding;
@property NSView *internalView;
- (void) createInternalView;
- (uint32_t *)currentBuffer;
- (uint32_t *)previousBuffer;
- (void)screenSizeChanged;
- (void)setRumble: (bool)on;
@end
