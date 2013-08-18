@implementation CocoaHorizontalSlider : NSSlider

-(id) initWith:(phoenix::HorizontalSlider&)horizontalSliderReference {
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
  if(horizontalSlider->onChange) horizontalSlider->onChange();
}

@end

namespace phoenix {

Size pHorizontalSlider::minimumSize() {
  return {48, 20};
}

unsigned pHorizontalSlider::position() {
  @autoreleasepool {
    return [cocoaView doubleValue];
  }
}

void pHorizontalSlider::setGeometry(Geometry geometry) {
  pWidget::setGeometry({
    geometry.x - 2, geometry.y,
    geometry.width + 4, geometry.height
  });
}

void pHorizontalSlider::setLength(unsigned length) {
  @autoreleasepool {
    [cocoaView setMaxValue:length];
  }
}

void pHorizontalSlider::setPosition(unsigned position) {
  @autoreleasepool {
    [cocoaView setDoubleValue:position];
  }
}

void pHorizontalSlider::constructor() {
  @autoreleasepool {
    cocoaView = cocoaHorizontalSlider = [[CocoaHorizontalSlider alloc] initWith:horizontalSlider];

    setLength(horizontalSlider.state.length);
    setPosition(horizontalSlider.state.position);
  }
}

void pHorizontalSlider::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
