#if defined(Hiro_Label)

@interface CocoaLabel : NSView {
@public
  hiro::mLabel* label;
}
-(id) initWith:(hiro::mLabel&)label;
-(void) resetCursorRects;
-(void) drawRect:(NSRect)dirtyRect;
-(void) mouseButton:(NSEvent*)event down:(BOOL)isDown;
-(void) mouseEntered:(NSEvent*)event;
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

struct pLabel : pWidget {
  Declare(Label, Widget)

  auto minimumSize() const -> Size override;
  auto setAlignment(Alignment alignment) -> void;
  auto setBackgroundColor(Color color) -> void;
  auto setForegroundColor(Color color) -> void;
  auto setText(const string& text) -> void;

  CocoaLabel* cocoaLabel = nullptr;
};

}

#endif
