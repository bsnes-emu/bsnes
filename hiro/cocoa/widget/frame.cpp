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
    [cocoaView removeFromSuperview];
    [cocoaView release];
  }
}

auto pFrame::append(sSizable sizable) -> void {
}

auto pFrame::remove(sSizable sizable) -> void {
}

auto pFrame::setEnabled(bool enabled) -> void {
  pWidget::setEnabled(enabled);
  if(auto sizable = _sizable()) sizable->setEnabled(sizable->self().enabled(true));
}

auto pFrame::setFont(const Font& font) -> void {
  @autoreleasepool {
    [cocoaView setTitleFont:pFont::create(font)];
  }
  if(auto sizable = _sizable()) sizable->setFont(sizable->self().font(true));
}

auto pFrame::setGeometry(Geometry geometry) -> void {
  bool empty = !state().text;
  Size size = pFont::size(self().font(true), state().text);
  pWidget::setGeometry({
    geometry.x() - 3, geometry.y() - (empty ? size.height() - 2 : 1),
    geometry.width() + 6, geometry.height() + (empty ? size.height() + 2 : 5)
  });
  if(auto sizable = _sizable()) {
    sizable->setGeometry({
      geometry.x() + 1, geometry.y() + (empty ? 1 : size.height() - 2),
      geometry.width() - 2, geometry.height() - (empty ? 1 : size.height() - 1)
    });
  }
}

auto pFrame::setText(const string& text) -> void {
  @autoreleasepool {
    [cocoaView setTitle:[NSString stringWithUTF8String:text]];
  }
}

auto pFrame::setVisible(bool visible) -> void {
  pWidget::setVisible(visible);
  if(auto sizable = _sizable()) sizable->setVisible(sizable->self().visible(true));
}

auto pFrame::_sizable() -> maybe<pSizable&> {
  if(auto sizable = state().sizable) {
    if(auto self = sizable->self()) return *self;
  }
  return {};
}

}

#endif
