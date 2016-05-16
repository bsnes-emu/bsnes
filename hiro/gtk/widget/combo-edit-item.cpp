#if defined(Hiro_ComboEdit)

namespace hiro {

auto pComboEditItem::construct() -> void {
}

auto pComboEditItem::destruct() -> void {
}

auto pComboEditItem::setIcon(const image& icon) -> void {
  if(auto parent = _parent()) {
    auto pixbuf = CreatePixbuf(icon, true);
    gtk_list_store_set(parent->gtkListStore, &gtkIter, 0, pixbuf, -1);
  }
}

auto pComboEditItem::setText(const string& text) -> void {
  if(auto parent = _parent()) {
    gtk_list_store_set(parent->gtkListStore, &gtkIter, 1, text.data(), -1);
  }
}

auto pComboEditItem::_parent() -> maybe<pComboEdit&> {
  if(auto parent = self().parentComboEdit()) {
    if(auto self = parent->self()) return *self;
  }
  return nothing;
}

}

#endif
