#import <Cocoa/Cocoa.h>
#import "Document.h"

@interface GBPaletteView : NSView
- (void)reloadData:(Document *)document;
@end
