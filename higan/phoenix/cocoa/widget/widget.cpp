namespace phoenix {

bool pWidget::enabled() {
  @autoreleasepool {
    return [cocoaView respondsToSelector:@selector(enabled)] && [cocoaView enabled];
  }
}

bool pWidget::focused() {
  @autoreleasepool {
    return cocoaView == [[cocoaView window] firstResponder];
  }
}

Size pWidget::minimumSize() {
  return {0, 0};
}

void pWidget::setEnabled(bool enabled) {
  if(widget.state.abstract) enabled = false;
  if(sizable.state.layout && sizable.state.layout->enabled() == false) enabled = false;

  @autoreleasepool {
    if([cocoaView respondsToSelector:@selector(setEnabled:)]) {
      [cocoaView setEnabled:enabled];
    }
  }
}

void pWidget::setFocused() {
  @autoreleasepool {
    [[cocoaView window] makeFirstResponder:cocoaView];
  }
}

void pWidget::setFont(const string &font) {
  @autoreleasepool {
    if([cocoaView respondsToSelector:@selector(setFont:)]) {
      [cocoaView setFont:pFont::cocoaFont(font)];
    }
  }
}

void pWidget::setGeometry(const Geometry &geometry) {
  @autoreleasepool {
    CGFloat windowHeight = [[cocoaView superview] frame].size.height;
    [cocoaView setFrame:NSMakeRect(geometry.x, windowHeight - geometry.y - geometry.height, geometry.width, geometry.height)];
    [[cocoaView superview] setNeedsDisplay:YES];
  }
}

void pWidget::setVisible(bool visible) {
  if(widget.state.abstract) visible = false;
  if(sizable.state.layout && sizable.state.layout->visible() == false) visible = false;

  @autoreleasepool {
    [cocoaView setHidden:!visible];
  }
}

void pWidget::constructor() {
  if(!widget.state.abstract) return;

  @autoreleasepool {
    cocoaView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, 0, 0)];
    [cocoaView setHidden:true];
  }
}

void pWidget::destructor() {
  @autoreleasepool {
    [cocoaView release];
  }
}

}
