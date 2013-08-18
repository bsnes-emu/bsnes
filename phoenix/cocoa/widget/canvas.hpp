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

  void setDroppable(bool droppable);
  void setSize(Size size);
  void update();

  pCanvas(Canvas& canvas) : pWidget(canvas), canvas(canvas) {}
  void constructor();
  void destructor();
};

}
