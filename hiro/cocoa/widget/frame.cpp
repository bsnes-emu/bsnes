#if defined(Hiro_Frame)

@implementation CocoaFrame : NSBox

-(id) initWith:(hiro::mFrame&)frameReference {
  if(self = [super initWithFrame:NSMakeRect(0, 0, 0, 0)]) {
    frame = &frameReference;

    [self setTitle:@""];
  }
  return self;
}

@end

namespace hiro {

auto pFrame::construct() -> void {
  @autoreleasepool {
    cocoaView = cocoaFrame = [[CocoaFrame alloc] initWith:self()];
    pWidget::construct();

    setText(state().text);
  }
}

auto pFrame::destruct() -> void {
  @autoreleasepool {
    [cocoaView release];
  }
}

auto pFrame::setEnabled(bool enabled) -> void {
  if(auto layout = _layout()) layout->setEnabled(layout->self().enabled(true));
  pWidget::setEnabled(enabled);
}

auto pFrame::setFont(const Font& font) -> void {
  @autoreleasepool {
    if(auto layout = _layout()) layout->setFont(layout->self().font(true));
    [cocoaView setTitleFont:pFont::create(font)];
  }
}

auto pFrame::setGeometry(Geometry geometry) -> void {
  bool empty = !state().text;
  Size size = pFont::size(self().font(true), state().text);
  pWidget::setGeometry({
    geometry.x() - 3, geometry.y() - (empty ? size.height() - 2 : 1),
    geometry.width() + 6, geometry.height() + (empty ? size.height() + 2 : 5)
  });
  if(auto layout = _layout()) {
    geometry.setX(geometry.x() + 1);
    geometry.setY(geometry.y() + (empty ? 1 : size.height() - 2));
    geometry.setWidth(geometry.width() - 2);
    geometry.setHeight(geometry.height() - (empty ? 1 : size.height() - 1));
    layout->setGeometry(geometry);
  }
}

auto pFrame::setText(const string& text) -> void {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

auto pFrame::setVisible(bool visible) -> void {
  if(auto layout = _layout()) layout->setVisible(layout->self().visible(true));
  pWidget::setVisible(visible);
}

auto pFrame::_layout() -> maybe<pLayout&> {
  if(auto layout = state().layout) {
    if(auto self = layout->self()) return *self;
  }
  return nothing;
}

}

#endif
