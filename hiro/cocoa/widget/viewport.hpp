#if defined(Hiro_Viewport)

@interface CocoaViewport : NSView {
@public
  hiro::mViewport* viewport;
}
-(id) initWith:(hiro::mViewport&)viewport;
-(void) drawRect:(NSRect)rect;
-(BOOL) acceptsFirstResponder;
-(NSDragOperation) draggingEntered:(id<NSDraggingInfo>)sender;
-(BOOL) performDragOperation:(id<NSDraggingInfo>)sender;
-(void) keyDown:(NSEvent*)event;
-(void) keyUp:(NSEvent*)event;
@end

namespace hiro {

struct pViewport : pWidget {
  Declare(Viewport, Widget)

  auto handle() const -> uintptr_t;
  auto setDroppable(bool droppable) -> void override;
  auto setFocusable(bool focusable) -> void override;

  CocoaViewport* cocoaViewport = nullptr;
};

}

#endif
