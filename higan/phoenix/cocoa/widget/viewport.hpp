@interface CocoaViewport : NSView {
@public
  phoenix::Viewport *viewport;
}
-(id) initWith:(phoenix::Viewport&)viewport;
-(void) drawRect:(NSRect)rect;
-(BOOL) acceptsFirstResponder;
-(void) keyDown:(NSEvent*)event;
-(void) keyUp:(NSEvent*)event;
@end

namespace phoenix {

struct pViewport : public pWidget {
  Viewport &viewport;
  CocoaViewport *cocoaViewport = nullptr;

  uintptr_t handle();

  pViewport(Viewport &viewport) : pWidget(viewport), viewport(viewport) {}
  void constructor();
  void destructor();
};

}
