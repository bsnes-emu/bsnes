#if defined(Hiro_Canvas)

@interface CocoaCanvas : NSImageView {
@public
  hiro::mCanvas* canvas;
}
-(id) initWith:(hiro::mCanvas&)canvas;
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

namespace hiro {

struct pCanvas : pWidget {
  Declare(Canvas, Widget)

  auto minimumSize() const -> Size;
  auto setColor(Color color) -> void;
  auto setDroppable(bool droppable) -> void;
  auto setGeometry(Geometry geometry) -> void override;
  auto setGradient(Gradient gradient) -> void;
  auto setIcon(const image& icon) -> void;
  auto update() -> void;

  auto _rasterize() -> void;
  auto _redraw() -> void;

  CocoaCanvas* cocoaCanvas = nullptr;
  uint surfaceWidth = 0;
  uint surfaceHeight = 0;
};

}

#endif
