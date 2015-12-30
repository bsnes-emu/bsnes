#if defined(Hiro_Label)

@implementation CocoaLabel : NSTextField

-(id) initWith:(hiro::mLabel&)labelReference {
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

namespace hiro {

auto pLabel::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaLabel = [[CocoaLabel alloc] initWith:self()];
    pWidget::construct();

    setText(state().text);
  }
}

auto pLabel::destruct() -> void {
  @autoreleasepool {
    [cocoaView release];
  }
}

auto pLabel::minimumSize() const -> Size {
  return pFont::size(self().font(true), state().text);
}

auto pLabel::setAlignment(Alignment alignment) -> void {
}

auto pLabel::setGeometry(Geometry geometry) -> void {
  //NSTextField does not support vertical text centering:
  //simulate this by adjusting the geometry placement (reduce height, move view down)
  uint height = pFont::size(self().font(true), " ").height();
  auto offset = geometry;

  if(geometry.height() > height) {
    uint diff = geometry.height() - height;
    offset.setY(offset.y() + diff >> 1);
    offset.setHeight(offset.height() - diff >> 1);
  }

  pWidget::setGeometry({
    offset.x() - 3, offset.y() - 3,
    offset.width() + 6, offset.height() + 6
  });
}

auto pLabel::setText(const string& text) -> void {
  @autoreleasepool {
    [cocoaView setStringValue:[NSString stringWithUTF8String:text]];
  }
}

}

#endif
