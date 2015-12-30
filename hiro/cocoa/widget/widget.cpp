#if defined(Hiro_Widget)

namespace hiro {

auto pWidget::construct() -> void {
  @autoreleasepool {
    if(!cocoaView) {
      abstract = true;
      cocoaView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 0, 0)];
      [cocoaView setHidden:true];
    }

    if(auto window = self().parentWindow(true)) {
      if(auto p = window->self()) p->_append(self());
      setEnabled(self().enabled(true));
      setFont(self().font(true));
      setVisible(self().visible(true));
    }
  }
}

auto pWidget::destruct() -> void {
  @autoreleasepool {
    [cocoaView release];
  }
}

/*
bool pWidget::enabled() {
  @autoreleasepool {
    return [cocoaView respondsToSelector:@selector(enabled)] && [cocoaView enabled];
  }
}
*/

auto pWidget::focused() const -> bool {
  @autoreleasepool {
    return cocoaView == [[cocoaView window] firstResponder];
  }
}

auto pWidget::setEnabled(bool enabled) -> void {
  if(abstract) enabled = false;

  @autoreleasepool {
    if([cocoaView respondsToSelector:@selector(setEnabled:)]) {
      [cocoaView setEnabled:enabled];
    }
  }
}

auto pWidget::setFocused() -> void {
  @autoreleasepool {
    [[cocoaView window] makeFirstResponder:cocoaView];
  }
}

auto pWidget::setFont(const Font& font) -> void {
  @autoreleasepool {
    if([cocoaView respondsToSelector:@selector(setFont:)]) {
      [cocoaView setFont:pFont::create(font)];
    }
  }
}

auto pWidget::setGeometry(Geometry geometry) -> void {
  @autoreleasepool {
    CGFloat windowHeight = [[cocoaView superview] frame].size.height;
    [cocoaView setFrame:NSMakeRect(geometry.x(), windowHeight - geometry.y() - geometry.height(), geometry.width(), geometry.height())];
    [[cocoaView superview] setNeedsDisplay:YES];
  }
  self().doSize();
}

auto pWidget::setVisible(bool visible) -> void {
  if(abstract) visible = false;

  @autoreleasepool {
    [cocoaView setHidden:!visible];
  }
}

}

#endif
