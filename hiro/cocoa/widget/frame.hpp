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

  auto append(sSizable sizable) -> void;
  auto remove(sSizable sizable) -> void;
  auto setEnabled(bool enabled) -> void override;
  auto setFont(const Font& font) -> void override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setText(const string& text) -> void;
  auto setVisible(bool visible) -> void override;

  auto _sizable() -> maybe<pSizable&>;

  CocoaFrame* cocoaFrame = nullptr;
};

}

#endif
