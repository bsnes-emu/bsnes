#if defined(Hiro_Frame)

@interface CocoaFrame : NSBox {
@public
  hiro::mFrame* frame;
}
-(id) initWith:(hiro::mFrame&)frame;
@end

namespace hiro {

struct pFrame : pWidget {
  Declare(Frame, Widget)

  auto append(sLayout layout) -> void;
  auto remove(sLayout layout) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFont(const Font& font) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setText(const string& text) -> void;
  auto setVisible(bool visible) -> void override;

  auto _layout() -> maybe<pLayout&>;

  CocoaFrame* cocoaFrame = nullptr;
};

}

#endif
