#import <Cocoa/Cocoa.h>
#import "Document.h"

@interface GBObjectView : NSView
- (void)reloadData:(Document *)document;
@end
