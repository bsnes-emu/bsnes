#if defined(Hiro_TabFrame)

namespace hiro {

auto pTabFrameItem::construct() -> void {
}

auto pTabFrameItem::destruct() -> void {
}

auto pTabFrameItem::append(sSizable sizable) -> void {
}

auto pTabFrameItem::remove(sSizable sizable) -> void {
}

auto pTabFrameItem::setClosable(bool closable) -> void {
}

auto pTabFrameItem::setIcon(const image& icon) -> void {
}

auto pTabFrameItem::setMovable(bool movable) -> void {
}

auto pTabFrameItem::setSelected() -> void {
  @autoreleasepool {
    if(auto parent = _parent()) {
      [parent->cocoaView selectTabViewItem:cocoaTabFrameItem];
    }
  }
}

auto pTabFrameItem::setText(const string& text) -> void {
  @autoreleasepool {
    [cocoaTabFrameItem setLabel:[NSString stringWithUTF8String:state().text]];
  }
}

auto pTabFrameItem::_parent() -> maybe<pTabFrame&> {
  if(auto parent = self().parentTabFrame()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
