#if defined(Hiro_IconView)

namespace hiro {

auto pIconViewItem::construct() -> void {
}

auto pIconViewItem::destruct() -> void {
}

auto pIconViewItem::setImage(const Image& image) -> void {
  if(auto parent = _parent()) {
    parent->setItemImage(self().offset(), image);
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
