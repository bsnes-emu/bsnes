@implementation CocoaVerticalSlider : NSSlider

-(id) initWith:(phoenix::VerticalSlider&)verticalSliderReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 1)]) {
    verticalSlider = &verticalSliderReference;

    [self setTarget:self];
    [self setAction:@selector(activate:)];
    [self setMinValue:0];
  }
  return self;
}

-(IBAction) activate:(id)sender {
  verticalSlider->state.position = [self doubleValue];
  if(verticalSlider->onChange) verticalSlider->onChange();
}

@end

namespace phoenix {

Size pVerticalSlider::minimumSize() {
  return {20, 48};
}

unsigned pVerticalSlider::position() {
  @autoreleasepool {
    return [cocoaView doubleValue];
  }
}

void pVerticalSlider::setGeometry(const Geometry &geometry) {
  pWidget::setGeometry({
    geometry.x, geometry.y - 2,
    geometry.width, geometry.height + 4
  });
}

void pVerticalSlider::setLength(unsigned length) {
  @autoreleasepool {
    [cocoaView setMaxValue:length];
  }
}

void pVerticalSlider::setPosition(unsigned position) {
  @autoreleasepool {
    [cocoaView setDoubleValue:position];
  }
}

void pVerticalSlider::constructor() {
  @autoreleasepool {
    cocoaView = cocoaVerticalSlider = [[CocoaVerticalSlider alloc] initWith:verticalSlider];

    setLength(verticalSlider.state.length);
    setPosition(verticalSlider.state.position);
  }
}

void pVerticalSlider::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
