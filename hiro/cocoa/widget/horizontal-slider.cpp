#if defined(Hiro_HorizontalSlider)

@implementation CocoaHorizontalSlider : NSSlider

-(id) initWith:(hiro::mHorizontalSlider&)horizontalSliderReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 1, 0)]) {
    horizontalSlider = &horizontalSliderReference;

    [self setTarget:self];
    [self setAction:@selector(activate:)];
    [self setMinValue:0];
  }
  return self;
}

-(IBAction) activate:(id)sender {
  horizontalSlider->state.position = [self doubleValue];
  horizontalSlider->doChange();
}

@end

namespace hiro {

auto pHorizontalSlider::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaHorizontalSlider = [[CocoaHorizontalSlider alloc] initWith:self()];
    pWidget::construct();

    setLength(state().length);
    setPosition(state().position);
  }
}

auto pHorizontalSlider::destruct() -> void {
  @autoreleasepool {
    [cocoaView removeFromSuperview];
    [cocoaView release];
  }
}

auto pHorizontalSlider::minimumSize() const -> Size {
  return {48, 20};
}

auto pHorizontalSlider::setGeometry(Geometry geometry) -> void {
  pWidget::setGeometry({
    geometry.x() - 2, geometry.y(),
    geometry.width() + 4, geometry.height()
  });
}

auto pHorizontalSlider::setLength(uint length) -> void {
  @autoreleasepool {
    [cocoaView setMaxValue:length];
  }
}

auto pHorizontalSlider::setPosition(uint position) -> void {
  @autoreleasepool {
    [cocoaView setDoubleValue:position];
  }
}

}

#endif
