#import <Cocoa/Cocoa.h>
#import <JoyKit/JoyKit.h>
#import "GBOSDView.h"
#import "GBViewBase.h"


@class Document;

@interface GBView : GBViewBase<JOYListener>
@property (nonatomic, weak) IBOutlet Document *document;
@property (nonatomic, getter=isMouseHidingEnabled) bool mouseHidingEnabled;
@property (nonatomic) bool isRewinding;
@property (weak) GBOSDView *osdView;
- (NSImage *)renderToImage;
- (void)setRumble: (double)amp;
@end
