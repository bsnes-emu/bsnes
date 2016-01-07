#if defined(Hiro_ComboButton)

namespace hiro {

auto pComboButtonItem::construct() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    parent->qtComboButton->addItem("");
    _setState();
    parent->unlock();
  }
}

auto pComboButtonItem::destruct() -> void {
  if(auto parent = _parent()) {
    parent->lock();
    parent->qtComboButton->removeItem(self().offset());
    parent->unlock();
  }
}

auto pComboButtonItem::setIcon(const image& icon) -> void {
  _setState();
}

auto pComboButtonItem::setSelected() -> void {
  _setState();
}

auto pComboButtonItem::setText(const string& text) -> void {
  _setState();
}

auto pComboButtonItem::_parent() -> maybe<pComboButton&> {
  if(auto parent = self().parentComboButton()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

auto pComboButtonItem::_setState() -> void {
  if(auto parent = _parent()) {
    parent->qtComboButton->setItemIcon(self().offset(), CreateIcon(state().icon));
    if(state().selected) parent->qtComboButton->setCurrentIndex(self().offset());
    parent->qtComboButton->setItemText(self().offset(), QString::fromUtf8(state().text));
  }
}

}

#endif
