#if defined(Hiro_StatusBar)

namespace hiro {

auto pStatusBar::construct() -> void {
}

auto pStatusBar::destruct() -> void {
}

auto pStatusBar::setEnabled(bool enabled) -> void {
  @autoreleasepool {
    if(auto parent = _parent()) {
      [[parent->cocoaWindow statusBar] setEnabled:enabled];
    }
  }
}

auto pStatusBar::setFont(const Font& font) -> void {
  @autoreleasepool {
    if(auto parent = _parent()) {
      [[parent->cocoaWindow statusBar] setFont:pFont::create(font)];
    }
  }
}

auto pStatusBar::setText(const string& text) -> void {
  @autoreleasepool {
    if(auto parent = _parent()) {
      [[parent->cocoaWindow statusBar] setStringValue:[NSString stringWithUTF8String:state().text]];
    }
  }
}

auto pStatusBar::setVisible(bool visible) -> void {
  @autoreleasepool {
    if(auto parent = _parent()) {
      [[parent->cocoaWindow statusBar] setHidden:!visible];
      parent->setGeometry(parent->state().geometry);
    }
  }
}

auto pStatusBar::_parent() -> maybe<pWindow&> {
  if(auto parent = self().parentWindow()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
