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
  if(!widget.parent()) enabled = false;
  if(widget.state.abstract) enabled = false;
  if(!widget.enabledToAll()) enabled = false;

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

void pWidget::setFont(string font) {
  @autoreleasepool {
    if([cocoaView respondsToSelector:@selector(setFont:)]) {
      [cocoaView setFont:pFont::cocoaFont(font)];
    }
  }
}

void pWidget::setGeometry(Geometry geometry) {
  @autoreleasepool {
    CGFloat windowHeight = [[cocoaView superview] frame].size.height;
    [cocoaView setFrame:NSMakeRect(geometry.x, windowHeight - geometry.y - geometry.height, geometry.width, geometry.height)];
    [[cocoaView superview] setNeedsDisplay:YES];
  }
  if(widget.onSize) widget.onSize();
}

void pWidget::setVisible(bool visible) {
  if(!widget.parent()) visible = false;
  if(widget.state.abstract) visible = false;
  if(!widget.visibleToAll()) visible = false;

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
