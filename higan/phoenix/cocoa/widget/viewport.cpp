@implementation CocoaViewport : NSView

-(id) initWith:(phoenix::Viewport&)viewportReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    viewport = &viewportReference;
  }
  return self;
}

-(void) drawRect:(NSRect)rect {
  [[NSColor blackColor] setFill];
  NSRectFillUsingOperation(rect, NSCompositeSourceOver);
}

-(BOOL) acceptsFirstResponder {
  return YES;
}

-(void) keyDown:(NSEvent*)event {
}

-(void) keyUp:(NSEvent*)event {
}

@end

namespace phoenix {

uintptr_t pViewport::handle() {
  return (uintptr_t)cocoaViewport;
}

void pViewport::constructor() {
  @autoreleasepool {
    cocoaView = cocoaViewport = [[CocoaViewport alloc] initWith:viewport];
  }
}

void pViewport::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
