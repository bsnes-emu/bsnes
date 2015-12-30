#if defined(Hiro_HorizontalSlider)

@interface CocoaHorizontalSlider : NSSlider {
@public
  hiro::mHorizontalSlider* horizontalSlider;
}
-(id) initWith:(hiro::mHorizontalSlider&)horizontalSlider;
-(IBAction) activate:(id)sender;
@end

namespace hiro {

struct pHorizontalSlider : pWidget {
  Declare(HorizontalSlider, Widget)

  auto minimumSize() const -> Size override;
  auto setGeometry(Geometry geometry) -> void;
  auto setLength(uint length) -> void;
  auto setPosition(uint position) -> void;

  CocoaHorizontalSlider* cocoaHorizontalSlider = nullptr;
};

}

#endif
