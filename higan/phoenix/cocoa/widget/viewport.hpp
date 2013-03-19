@interface CocoaViewport : NSView {
@public
  phoenix::Viewport *viewport;
}
-(id) initWith :(phoenix::Viewport&)viewport;
-(void) drawRect :(NSRect)rect;
@end

namespace phoenix {

struct pViewport : public pWidget {
  Viewport &viewport;
  CocoaViewport *cocoaViewport;

  uintptr_t handle();

  pViewport(Viewport &viewport) : pWidget(viewport), viewport(viewport) {}
  void constructor();
  void destructor();
};

}
