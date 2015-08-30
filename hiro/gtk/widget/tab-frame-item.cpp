#if defined(Hiro_TabFrame)

namespace hiro {

auto pTabFrameItem::construct() -> void {
  if(auto layout = state().layout) layout->construct();
}

auto pTabFrameItem::destruct() -> void {
  if(auto layout = state().layout) layout->destruct();
}

auto pTabFrameItem::append(sLayout layout) -> void {
}

auto pTabFrameItem::remove(sLayout layout) -> void {
}

auto pTabFrameItem::setClosable(bool closable) -> void {
  if(auto parent = _parent()) {
    parent->setItemClosable(self().offset(), closable);
  }
}

auto pTabFrameItem::setImage(const Image& image) -> void {
  if(auto parent = _parent()) {
    parent->setItemImage(self().offset(), image);
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
