#if defined(Hiro_TabFrame)

namespace hiro {

auto pTabFrameItem::construct() -> void {
  if(auto layout = state().layout) layout->construct();
}

auto pTabFrameItem::destruct() -> void {
  if(auto layout = state().layout) layout->destruct();
}

auto pTabFrameItem::setClosable(bool closable) -> void {
  if(auto parent = _parent()) {
    parent->setItemClosable(self().offset(), closable);
  }
}

auto pTabFrameItem::setIcon(const image& icon) -> void {
  if(auto parent = _parent()) {
    parent->setItemIcon(self().offset(), icon);
  }
}

auto pTabFrameItem::setMovable(bool movable) -> void {
  if(auto parent = _parent()) {
    parent->setItemMovable(self().offset(), movable);
  }
}

auto pTabFrameItem::setSelected() -> void {
  if(auto parent = _parent()) {
    parent->setItemSelected(self().offset());
  }
}

auto pTabFrameItem::setText(const string& text) -> void {
  if(auto parent = _parent()) {
    parent->setItemText(self().offset(), text);
  }
}

auto pTabFrameItem::_parent() -> pTabFrame* {
  if(auto parent = self().parentTabFrame()) return parent->self();
  return nullptr;
}

}

#endif
