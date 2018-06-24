#if defined(Hiro_Label)

@implementation CocoaLabel : NSTextView

-(id) initWith:(hiro::mLabel&)labelReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    label = &labelReference;

    [self setDrawsBackground:NO];
    [self setEditable:NO];
    [self setRichText:NO];
  }
  return self;
}

@end

namespace hiro {

auto pLabel::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaLabel = [[CocoaLabel alloc] initWith:self()];
    pWidget::construct();

    setAlignment(state().alignment);
    setText(state().text);
  }
}

auto pLabel::destruct() -> void {
  @autoreleasepool {
    [cocoaView removeFromSuperview];
    [cocoaView release];
  }
}

auto pLabel::minimumSize() const -> Size {
  return pFont::size(self().font(true), state().text);
}

auto pLabel::setAlignment(Alignment alignment) -> void {
  @autoreleasepool {
    NSMutableParagraphStyle* paragraphStyle = [[NSParagraphStyle defaultParagraphStyle] mutableCopy];
    paragraphStyle.alignment = NSTextAlignmentCenter;
    if(alignment.horizontal() < 0.333) paragraphStyle.alignment = NSTextAlignmentLeft;
    if(alignment.horizontal() > 0.666) paragraphStyle.alignment = NSTextAlignmentRight;
    [cocoaView setDefaultParagraphStyle:paragraphStyle];
  }
}

auto pLabel::setBackgroundColor(Color color) -> void {
  //todo
}

auto pLabel::setForegroundColor(Color color) -> void {
  //todo
}

auto pLabel::setGeometry(Geometry geometry) -> void {
  //NSTextView does not support vertical text centering:
  //simulate this by adjusting the geometry placement (reduce height, move view down)
  uint height = pFont::size(self().font(true), state().text).height();
  auto offset = geometry;

  if(geometry.height() > height) {
    uint diff = geometry.height() - height;
    offset.setY(offset.y() + (diff >> 1));
    offset.setHeight(offset.height() - (diff >> 1));
  }

  pWidget::setGeometry({
    offset.x() - 6, offset.y(),
    offset.width() + 12, offset.height()
  });
}

auto pLabel::setText(const string& text) -> void {
  @autoreleasepool {
    [cocoaView setString:[NSString stringWithUTF8String:text]];
  }
}

}

#endif
