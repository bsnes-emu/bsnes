@interface CocoaViewport : NSView {
@public
  phoenix::Viewport* viewport;
}
-(id) initWith:(phoenix::Viewport&)viewport;
-(void) drawRect:(NSRect)rect;
-(BOOL) acceptsFirstResponder;
-(NSDragOperation) draggingEntered:(id<NSDraggingInfo>)sender;
-(BOOL) performDragOperation:(id<NSDraggingInfo>)sender;
-(void) keyDown:(NSEvent*)event;
-(void) keyUp:(NSEvent*)event;
@end

namespace phoenix {

struct pViewport : public pWidget {
  Viewport& viewport;
  CocoaViewport* cocoaViewport = nullptr;

  uintptr_t handle();
  void setDroppable(bool droppable);

  pViewport(Viewport& viewport) : pWidget(viewport), viewport(viewport) {}
  void constructor();
  void destructor();
};

}
