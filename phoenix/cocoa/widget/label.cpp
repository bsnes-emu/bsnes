@implementation CocoaLabel : NSTextField

-(id) initWith:(phoenix::Label&)labelReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    label = &labelReference;

    [self setAlignment:NSLeftTextAlignment];
    [self setBordered:NO];
    [self setDrawsBackground:NO];
    [self setEditable:NO];
  }
  return self;
}

@end

namespace phoenix {

Size pLabel::minimumSize() {
  Size size = Font::size(label.font(), label.state.text);
  return {size.width, size.height};
}

void pLabel::setGeometry(Geometry geometry) {
  //NSTextField does not support vertical text centering:
  //simulate this by adjusting the geometry placement (reduce height, move view down)
  unsigned height = Font::size(label.font(), " ").height;
  unsigned widgetHeight = geometry.height + 4;  //+4 compensates for margin adjust
  auto offset = geometry;

  if(widgetHeight > height) {
    unsigned diff = widgetHeight - height;
    offset.y += diff >> 1;
    offset.height -= diff >> 1;
  }

  pWidget::setGeometry({
    offset.x - 3, offset.y - 3,
    offset.width + 6, offset.height + 6
  });
}

void pLabel::setText(string text) {
  @autoreleasepool {
    [cocoaView setStringValue:[NSString stringWithUTF8String:text]];
  }
}

void pLabel::constructor() {
  @autoreleasepool {
    cocoaView = cocoaLabel = [[CocoaLabel alloc] initWith:label];
    setText(label.state.text);
  }
}

void pLabel::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
