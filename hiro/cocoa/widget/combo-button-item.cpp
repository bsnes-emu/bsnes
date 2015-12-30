#if defined(Hiro_ComboButton)

namespace hiro {

auto pComboButtonItem::construct() -> void {
}

auto pComboButtonItem::destruct() -> void {
}

auto pComboButtonItem::setImage(const Image& icon) -> void {
}

auto pComboButtonItem::setSelected() -> void {
  @autoreleasepool {
    if(auto parent = _parent()) {
      [parent->cocoaView selectItemAtIndex:self().offset()];
    }
  }
}

auto pComboButtonItem::setText(const string& text) -> void {
  @autoreleasepool {
    if(auto parent = _parent()) {
      [[parent->cocoaView itemAtIndex:self().offset()] setTitle:[NSString stringWithUTF8String:text]];
    }
  }
}

auto pComboButtonItem::_parent() -> maybe<pComboButton&> {
  if(auto parent = self().parentComboButton()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
