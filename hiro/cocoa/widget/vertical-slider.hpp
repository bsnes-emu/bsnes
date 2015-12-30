#if defined(Hiro_VerticalSlider)

@interface CocoaVerticalSlider : NSSlider {
@public
  hiro::mVerticalSlider* verticalSlider;
}
-(id) initWith:(hiro::mVerticalSlider&)verticalSlider;
-(IBAction) activate:(id)sender;
@end

namespace hiro {

struct pVerticalSlider : pWidget {
  Declare(VerticalSlider, Widget)

  auto minimumSize() const -> Size override;
  auto setGeometry(Geometry geometry) -> void override;
  auto setLength(uint length) -> void;
  auto setPosition(uint position) -> void;

  CocoaVerticalSlider* cocoaVerticalSlider = nullptr;
};

}

#endif
