#if defined(Hiro_TabFrame)

namespace hiro {

auto pTabFrameItem::construct() -> void {
  if(auto parent = _parent()) parent->_append();
  if(auto& sizable = state().sizable) sizable->construct();
}

auto pTabFrameItem::destruct() -> void {
  if(auto& sizable = state().sizable) sizable->destruct();
}

auto pTabFrameItem::append(sSizable sizable) -> void {
}

auto pTabFrameItem::remove(sSizable sizable) -> void {
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

auto pTabFrameItem::_parent() -> maybe<pTabFrame&> {
  if(auto parent = self().parentTabFrame()) {
    if(auto self = parent->self()) return *self;
  }
  return {};
}

}

#endif
