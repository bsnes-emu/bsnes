#if defined(Hiro_IconView)

namespace hiro {

auto pIconViewItem::construct() -> void {
}

auto pIconViewItem::destruct() -> void {
}

auto pIconViewItem::setIcon(const image& icon) -> void {
  if(auto parent = _parent()) {
    parent->setItemIcon(self().offset(), icon);
  }
}

auto pIconViewItem::setSelected(bool selected) -> void {
  if(auto parent = _parent()) {
    parent->setItemSelected(self().offset(), selected);
  }
}

auto pIconViewItem::setText(const string& text) -> void {
  if(auto parent = _parent()) {
    parent->setItemText(self().offset(), text);
  }
}

auto pIconViewItem::_parent() -> pIconView* {
  if(auto parent = self().parentIconView()) return parent->self();
  return nullptr;
}

}

#endif
