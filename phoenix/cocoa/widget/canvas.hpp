@interface CocoaCanvas : NSImageView {
@public
  phoenix::Canvas* canvas;
}
-(id) initWith:(phoenix::Canvas&)canvas;
-(NSDragOperation) draggingEntered:(id<NSDraggingInfo>)sender;
-(BOOL) performDragOperation:(id<NSDraggingInfo>)sender;
-(void) mouseButton:(NSEvent*)event down:(BOOL)isDown;
-(void) mouseExited:(NSEvent*)event;
-(void) mouseMove:(NSEvent*)event;
-(void) mouseDown:(NSEvent*)event;
-(void) mouseUp:(NSEvent*)event;
-(void) mouseDragged:(NSEvent*)event;
-(void) rightMouseDown:(NSEvent*)event;
-(void) rightMouseUp:(NSEvent*)event;
-(void) rightMouseDragged:(NSEvent*)event;
-(void) otherMouseDown:(NSEvent*)event;
-(void) otherMouseUp:(NSEvent*)event;
-(void) otherMouseDragged:(NSEvent*)event;
@end

namespace phoenix {

struct pCanvas : public pWidget {
  Canvas& canvas;
  CocoaCanvas* cocoaCanvas = nullptr;
  unsigned surfaceWidth = 0;
  unsigned surfaceHeight = 0;

  void setDroppable(bool droppable);
  void setGeometry(Geometry geometry);
  void setMode(Canvas::Mode mode);
  void setSize(Size size);

  pCanvas(Canvas& canvas) : pWidget(canvas), canvas(canvas) {}
  void constructor();
  void destructor();
  void rasterize();
  void redraw();
};

}
