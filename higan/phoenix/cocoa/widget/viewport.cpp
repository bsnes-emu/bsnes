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

-(NSDragOperation) draggingEntered:(id<NSDraggingInfo>)sender {
  return DropPathsOperation(sender);
}

-(BOOL) performDragOperation:(id<NSDraggingInfo>)sender {
  lstring paths = DropPaths(sender);
  if(paths.empty()) return NO;
  if(viewport->onDrop) viewport->onDrop(paths);
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

void pViewport::setDroppable(bool droppable) {
  @autoreleasepool {
    if(droppable) {
      [cocoaViewport registerForDraggedTypes:[NSArray arrayWithObject:NSFilenamesPboardType]];
    } else {
      [cocoaViewport unregisterDraggedTypes];
    }
  }
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
